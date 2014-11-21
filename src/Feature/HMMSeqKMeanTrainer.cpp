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

HMMSeqKMeanTrainer::HMMSeqKMeanTrainer() {

}

HMMSeqKMeanTrainer::~HMMSeqKMeanTrainer() {

}

void HMMSeqKMeanTrainer::hmmSeqTrain() {
    initHMMTrain();

    buildModels();

    int trainTime = MAX_TRAIN_TIMES;

    while(trainTime --) {
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

//    printf("%p\n", &((*this->mixedWavs)[0]));
//    printf("%d\n", (*this->mixedWavs)[0].size());
//    printf("%p\n", (trainWavs[0].wav));
//    printf("%d\n", (trainWavs[0].wav)->size());

    // do dtw to search best path
    for(wavIdx = 0; wavIdx < trainWavs.size(); wavIdx ++) {
        (trainWavs[wavIdx].model)->reSegment( *(trainWavs[wavIdx].wav), wavIdx );
    }

    bool bigChange = false;
    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
        HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(automatonVec[autoIdx]);

        // update parameters!!
        bigChange |= automaton[autoIdx].updateTransfer();
        automaton[autoIdx].iterateGauss();
    }

    refreshModels();

    return bigChange;
}

void HMMSeqKMeanTrainer::initHMMTrain() {
    std::vector<WaveFeatureOP> & templates = * mixedWavs;
    std::map<std::string, HMMAutomaton *> & automatons = *(this->automatons);
    std::map<std::string, HMMAutomaton *>::iterator autoItr;

    int templateIdx;
    int templateSiz = templates.size();
    int autoIdx;

    
    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
//    for(autoItr = automatons.begin(); autoItr != automatons.end(); autoItr ++) {
        HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(automatonVec[autoIdx]); // autoItr->second);

        //malloc transfer and state for every automaton
        automaton->hmmMallocStatesAndTransfer();

        // all state has not points before first segmentation
        automaton->allEmptyStateSegment();
    }

    std::vector< std::string > words;

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

    for(autoIdx = 0; autoIdx < automatonVec.size(); autoIdx ++) {
//    for(autoItr = automatons.begin(); autoItr != automatons.end(); autoItr ++) {
        HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(automatonVec[autoIdx]); // autoItr->second);
//    for(autoItr = automatons.begin(); autoItr != automatons.end(); autoItr ++) {
//        HMMKMeanAutomaton * automaton = (HMMKMeanAutomaton *)(autoItr->second);

        // init transfer and apply gauss base on the init segmentation
        automaton->initTransfer();
        automaton->iterateGauss();
    }
}
