// =====================================================================================
// 
//       Filename:  HMMSeqRecognition.cpp
// 
//    Description:  <F4>
// 
//        Version:  0.01
//        Created:  2014/11/18 21时43分03秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "HMMSeqRecognition.h"
#include <fstream>

HMMSeqRecognition::HMMSeqRecognition(int stateNum, int gaussNum, int trainTimes) : HMMRecognition(stateNum, gaussNum, trainTimes) {

}

HMMSeqRecognition::~HMMSeqRecognition() {

}

void HMMSeqRecognition::hmmSegTrain() {
    hmmTrain(HMMAutomaton::SEG);
}

void HMMSeqRecognition::hmmSeqTrain() {
    hmmTrain(HMMAutomaton::SEQ);
}

void HMMSeqRecognition::loadGraph(const char *filename) {
    graph.parseGraph(filename);

    buildSeqModel();
}

void HMMSeqRecognition::buildSeqModel() {
    seqModel.buildModel( graph, automatons.getAutomatons());
}

void HMMSeqRecognition::dumpDot(std::ofstream &out) {
    seqModel.dumpDot(out);
}

void HMMSeqRecognition::setBeam( double beam ) {
    seqModel.setBeam( beam );
}

void HMMSeqRecognition::recognition(WaveFeatureOP & input, std::vector<std::string> & res, SeqModel::SEQ_DTW_PATH_TYPE pathType) {
    seqModel.recognition(input, res, pathType);
}

void HMMSeqRecognition::close() {
    automatons.clear();

    seqModel.close();
}

