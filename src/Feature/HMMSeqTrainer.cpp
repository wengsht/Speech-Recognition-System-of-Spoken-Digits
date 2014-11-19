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

HMMSeqTrainer::HMMSeqTrainer() {

}

HMMSeqTrainer::~HMMSeqTrainer() {

}

void HMMSeqTrainer::buildModels(WaveFeatureOPSet::dataSetType & dataSet, std::map<std::string, HMMAutomaton *> & automatons, std::vector<WaveFeatureOP *> &mixedWavs) {
    WaveFeatureOPSet::dataSetType::iterator templateItr;

    mixedWavs.clear();
    int idx;
    for(templateItr = dataSet.begin(); templateItr != dataSet.end(); templateItr ++) {
        pushNewModel(templateItr->first, automatons, trainModels);

        std::vector<WaveFeatureOP> & wavs = templateItr->second;

        for(idx = 0; idx < wavs.size(); idx++) {
            mixedWavs.push_back(&(wavs[idx]));

            if(trainModels.size())
                trainWavs.push_back(SeqWav(&(wavs[idx]), &(trainModels[trainModels.size() - 1])));
        }
    }
}

void HMMSeqTrainer::hmmSeqTrain() {
    //TODO

    // trainModels[??].train(trainWavs[idx], idx);
    // updateSeg for idxth wav in hmmstate *
}

void HMMSeqTrainer::pushNewModel(const std::string &seqStr,std::map<std::string, HMMAutomaton *> &automatons, std::vector<SeqModel> &models) {
    ParseGraph graph;

    graph.parseSeqStr(seqStr);

    trainModels.push_back(SeqModel());

    int idx = trainModels.size() - 1;

    trainModels[idx].buildModel(graph, automatons);
}
