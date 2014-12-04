// =====================================================================================
// 
//       Filename:  HMMSeqTrainer.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/19 02时14分07秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "HMMSeqTrainer.h"
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include "HMMAutomaton.h"
#include "WaveFeatureOPSet.h"
#include "WaveFeatureOP.h"

#include "ThreadPool.h"

HMMSeqTrainer::HMMSeqTrainer() {

}

HMMSeqTrainer::~HMMSeqTrainer() {

}

void HMMSeqTrainer::buildModels(WaveFeatureOPSet::dataSetType & dataSet, std::map<std::string, HMMAutomaton *> & automatons, std::vector<WaveFeatureOP> &mixedWavs) {
    Log("Begin Build sequential Models");

    this->automatons = & automatons;
    this->mixedWavs = & mixedWavs;

    WaveFeatureOPSet::dataSetType::iterator templateItr;

    Log("Combine all the wav files");

    mixedWavs.clear();
    int idx;
    for(templateItr = dataSet.begin(); templateItr != dataSet.end(); templateItr ++) {
        pushNewModel(templateItr->first, automatons, trainModels);

        std::vector<WaveFeatureOP> & wavs = templateItr->second;

        for(idx = 0; idx < wavs.size(); idx++) {
            mixedWavs.push_back(wavs[idx]);
        }
    }

    trainWavs.resize(mixedWavs.size());

    int idy = 0, idz = 0;
    for(templateItr = dataSet.begin(); templateItr != dataSet.end(); templateItr ++, idz ++ ) {
        std::vector<WaveFeatureOP> & wavs = templateItr->second;
        for(idx = 0; idx < wavs.size(); idx++, idy ++) {
            trainWavs[idy].wav = &(mixedWavs[idy]);
            trainWavs[idy].model = &(trainModels[idz]);
        }
    }

    std::map<std::string, HMMAutomaton *>::iterator Itr;

    automatonVec.clear();

    for(Itr = automatons.begin(); Itr != automatons.end(); Itr++) {
        (Itr->second)->setTemplates(& mixedWavs);

        automatonVec.push_back( Itr->second );
    }

    hmmSeqTrain();
}

void HMMSeqTrainer::pushNewModel(const std::string &seqStr,std::map<std::string, HMMAutomaton *> &automatons, std::vector<SeqModel> &models) {
    ParseGraph graph;

    //    std::cout << templateItr->first << std::endl;
    //    std::cout << seqStr << std::endl;
    graph.parseSeqStr(seqStr);

    trainModels.push_back( SeqModel() );
    graphs.push_back(graph);

    //    int idx = trainModels.size() - 1;

    //    trainModels[idx].buildModel(graph, automatons);
}

void HMMSeqTrainer::buildModelTask(void *in) {
    BuildInfo * info = (BuildInfo *) in;
    SeqModel * model = info->model;
    ParseGraph * graph = info->graph;
    std::map<std::string, HMMAutomaton *> * automatons = info->automatons;

    model->buildModel( *graph, *automatons);
    //trainModels[modelIdx].refreshEdges( graphs[modelIdx], *automatons);
}

void HMMSeqTrainer::buildModels() {
    assert(trainModels.size() == graphs.size());
    int modelIdx;
    ThreadPool threadPool(ThreadPool::thread_num);
    for(modelIdx = 0; modelIdx < trainModels.size(); modelIdx++) {
        sp_task task;
        BuildInfo * buildInfo = new BuildInfo;

        buildInfo->model = &(trainModels[modelIdx]);
        buildInfo->graph = &(graphs[modelIdx]);
        buildInfo->automatons = automatons;

        task.func = buildModelTask;
        task.in = (void *) buildInfo;

        threadPool.addTask(task);
    }
    threadPool.run();

    /*  
    for(modelIdx = 0; modelIdx < trainModels.size(); modelIdx++) 
        trainModels[modelIdx].buildModel( graphs[modelIdx], *automatons);

        */
}

void HMMSeqTrainer::refreshModelTask(void *in) {
    RefreshInfo * info = (RefreshInfo *) in;
    SeqModel * model = info->model;
    ParseGraph * graph = info->graph;
    std::map<std::string, HMMAutomaton *> * automatons = info->automatons;

    model->refreshEdges( *graph, *automatons);
    //trainModels[modelIdx].refreshEdges( graphs[modelIdx], *automatons);
}
void HMMSeqTrainer::refreshModels() {
    assert(trainModels.size() == graphs.size());
    int modelIdx;

    ThreadPool threadPool(ThreadPool::thread_num);

    for(modelIdx = 0; modelIdx < trainModels.size(); modelIdx++) {
        sp_task task;
        RefreshInfo * refreshInfo = new RefreshInfo;

        refreshInfo->model = &(trainModels[modelIdx]);
        refreshInfo->graph = &(graphs[modelIdx]);
        refreshInfo->automatons = automatons;

        task.func = refreshModelTask;
        task.in = (void *) refreshInfo;

        threadPool.addTask(task);
    }
    threadPool.run();

    /*  
    for(modelIdx = 0; modelIdx < trainModels.size(); modelIdx++) 
        trainModels[modelIdx].refreshEdges( graphs[modelIdx], *automatons);
        */
}
