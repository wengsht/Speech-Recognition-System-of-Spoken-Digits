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

HMMSeqTrainer::HMMSeqTrainer() {

}

HMMSeqTrainer::~HMMSeqTrainer() {

}

void HMMSeqTrainer::buildModels(WaveFeatureOPSet::dataSetType & dataSet, std::map<std::string, HMMAutomaton *> & automatons, std::vector<WaveFeatureOP> &mixedWavs) {
    this->automatons = & automatons;
    this->mixedWavs = & mixedWavs;

    WaveFeatureOPSet::dataSetType::iterator templateItr;

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

void HMMSeqTrainer::buildModels() {
    assert(trainModels.size() == graphs.size());
    int modelIdx;
    for(modelIdx = 0; modelIdx < trainModels.size(); modelIdx++) 
        trainModels[modelIdx].buildModel( graphs[modelIdx], *automatons);

    /*  
        std::ofstream out("11.dot");
        trainModels[0].dumpDot(out);
        out.close();
        */
}

void HMMSeqTrainer::refreshModels() {
    assert(trainModels.size() == graphs.size());
    int modelIdx;
    for(modelIdx = 0; modelIdx < trainModels.size(); modelIdx++) 
        trainModels[modelIdx].refreshEdges( graphs[modelIdx], *automatons);
}
