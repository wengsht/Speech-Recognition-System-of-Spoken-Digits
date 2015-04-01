// =====================================================================================
// 
//       Filename:  HMMSeqKMeanTrainer.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/19 08时46分36秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "HMMSeqKMeanTrainer.h"
#include "HMMKMeanAutomaton.h"

#include "parseGraph.h"
#include "configure_hmm.h"
#include "SeqModel.h"

#include "ThreadPool.h"

HMMSeqKMeanTrainer::HMMSeqKMeanTrainer() {

}

HMMSeqKMeanTrainer::~HMMSeqKMeanTrainer() {

}

void HMMSeqKMeanTrainer::hmmSeqTrain() {
    Log("Starting Sequtial Train!");

    initHMMTrain();

    Log("Building Graphs!");

    buildModels();

    int trainTime = MAX_TRAIN_TIMES;

    while(trainTime --) {
        Log("Train: [%d]", trainTime);

        if(! iteratorSeqTrain())
            break;
    }
}

bool HMMSeqKMeanTrainer::iteratorSeqTrain() {
    std::map<std::string, HMMAutomaton *> & automatons = *(this->automatons);
    std::map<std::string, HMMAutomaton *>::iterator autoItr;
    int autoIdx;
    int wavIdx;

    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
        HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(automatonVec[autoIdx]);

        // it is easy to segment if we empty all states firstly
        automaton->allEmptyStateSegment();
    }

    // do dtw to search best path
    /// TODO Multi thread 
    int threadNum = ThreadPool::thread_num;
    if(threadNum > trainWavs.size()) threadNum = trainWavs.size();

    ThreadPool threadPool(threadNum);

    int threadIdx;
    for(threadIdx = 0; threadIdx < threadNum; threadIdx++) {
        struct sp_task task;
        struct segTaskInfo * info = new segTaskInfo;

        info->trainWavs = & trainWavs;
        info->threadID  = threadIdx;
        info->threadNum = threadNum;

        task.func = segmentTask;
        task.in   = (void *) info;

        threadPool.addTask(task);
    }
    threadPool.run();

    Log("reSegment Success!");
    /*  
    for(wavIdx = 0; wavIdx < trainWavs.size(); wavIdx ++) {
        (trainWavs[wavIdx].model)->reSegment( *(trainWavs[wavIdx].wav), wavIdx );
    }
    */

    bool bigChange = false;
    /// TODO Multi thread
    threadPool.clear();
    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
        struct sp_task task;
        struct updateTaskInfo * info = new updateTaskInfo;

        info->bigChange = & bigChange;
        info->automaton = (HMMKMeanAutomaton *)(automatonVec[autoIdx]);

        task.func = updateParaTask;
        task.in   = (void *) info;

        threadPool.addTask(task);
    }
    threadPool.run();

    /*  
    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
        HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(automatonVec[autoIdx]);

        // update parameters!!
        bigChange |= automaton->updateTransfer();

        automaton->iterateGauss();
    }
    */

    refreshModels();

    return bigChange;
}

pthread_mutex_t HMMSeqKMeanTrainer::lock = PTHREAD_MUTEX_INITIALIZER;

void HMMSeqKMeanTrainer::updateParaTask( void * in) {
    struct updateTaskInfo *info = (updateTaskInfo *) in;

    bool &bigChange = *(info->bigChange);
    bool taskBigChange = false;

    HMMKMeanAutomaton * automaton = info->automaton;

    taskBigChange = automaton->updateTransfer();

    automaton->iterateGauss();

    pthread_mutex_lock(&lock);
    bigChange |= taskBigChange;
    pthread_mutex_unlock(&lock);

    delete info;
}

int HMMSeqKMeanTrainer::addressTag(void * address, int range) {
    unsigned long addr = (unsigned long) address;

    return (addr >> 3) % range;
}
void HMMSeqKMeanTrainer::segmentTask( void * in) {
    struct segTaskInfo *info = (segTaskInfo *) in;

    int threadID = info->threadID;
    int threadNum = info->threadNum;

    std::vector<SeqWav> & trainWavs = *(info->trainWavs);

    int wavIdx;
    for(wavIdx = 0; wavIdx < trainWavs.size(); wavIdx ++) {
        // model should not be call in different threads!!
        if(threadID == addressTag(trainWavs[wavIdx].model, threadNum))
            (trainWavs[wavIdx].model)->reSegment( *(trainWavs[wavIdx].wav), wavIdx );
    }

    delete info;

//    (trainWavs[wavIdx].model)->reSegment( *(trainWavs[wavIdx].wav), wavIdx );
}

void HMMSeqKMeanTrainer::initParaTask( void * in) {
    HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *) in;

    if(! automaton->isLoadFromFile()) {
        Log("Automaton [%p] init Transfer", in);
        automaton->initTransfer();
        Log("Automaton [%p] init Gaussian", in);
        automaton->iterateGauss();

        automaton->setIsLoadFromFile(false);
    }
}

void HMMSeqKMeanTrainer::initHMMTrain() {
    std::vector<WaveFeatureOP> & templates = * mixedWavs;
    std::map<std::string, HMMAutomaton *> & automatons = *(this->automatons);
    std::map<std::string, HMMAutomaton *>::iterator autoItr;

    int templateIdx;
    int templateSiz = templates.size();
    int autoIdx;

    Log("Malloc HMM State and Transfer Cost Matrix");
    
    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
        HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(automatonVec[autoIdx]); // autoItr->second);

        // automaton that load from file already has transfer matrix and states, don't generate again
        if(! automaton->isLoadFromFile()) {
            //malloc transfer and state for every automaton
            automaton->hmmMallocStatesAndTransfer();
        }

        // all state has not points before first segmentation
        automaton->allEmptyStateSegment();
    }

    std::vector< std::string > words;

    Log("Init Segment");
    // first segmentation
    for(templateIdx = 0; templateIdx < templateSiz; templateIdx++) {
        int wavSiz = templates[templateIdx].size();
        words.clear();
        ParseGraph::dumpSeqStr2Vec(templates[templateIdx].getWord(), words);

        int wordSiz = words.size();
        if(wordSiz < 1) wordSiz = 1;
        int stepLen = wavSiz / wordSiz;

        int startI = 0, endI = 0;
        
        for(int wordIdx = 0; wordIdx < words.size(); wordIdx ++) {
            autoItr = automatons.find(words[wordIdx]);

            endI = startI + stepLen - 1;
            if(wordIdx == words.size() - 1)
                endI = wavSiz - 1;

            if(autoItr != automatons.end()) {
                HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(autoItr->second);

                automaton->initSegment(templateIdx, startI, endI);
            }

            startI = endI + 1;
        }
    }

    ThreadPool threadPool(ThreadPool::thread_num);
    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
        sp_task task;
        task.func = initParaTask;
        task.in   = (void *) automatonVec[autoIdx];

        threadPool.addTask(task);
    }
    threadPool.run();

    /*  
    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
        HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(automatonVec[autoIdx]); // autoItr->second);

        // init transfer and apply gauss base on the init segmentation
        Log("Automaton [%d] init Transfer", autoIdx);
        automaton->initTransfer();
        Log("Automaton [%d] init Gaussian", autoIdx);
        automaton->iterateGauss();
    }
    */
}
