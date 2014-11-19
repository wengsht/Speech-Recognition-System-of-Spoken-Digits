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

    std::ofstream out;
    out.open("./model.dot");
    seqModel.dumpDot(out);
    out.close();
}

void HMMSeqRecognition::recognition(WaveFeatureOP & input, std::vector<std::string> & res) {
    seqModel.recognition(input, res);
}

void HMMSeqRecognition::close() {
    automatons.clear();

    seqModel.close();
}

