// =====================================================================================
// 
//       Filename:  HMMKmeanAutomaton.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/21 22时36分49秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "HMMSoftAutomaton.h"
#include "SoftState.h"

HMMSoftAutomaton::HMMSoftAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum, int gaussNum, int trainTimes) : HMMAutomaton(templates, stateNum, gaussNum, trainTimes) {
}
HMMSoftAutomaton::~HMMSoftAutomaton() {
}

void HMMSoftAutomaton::hmmTrain() {
    states.clear();

    std::vector<WaveFeatureOP> & datas = *templates;

    int idx, idy, idz;
    // 初始化 几个states
    for(idx = 0;idx < stateNum;idx ++) {
        states.push_back(SoftState(templates));
    }

    for(idx = 0; idx < datas.size(); idx++) {
        for(int idy = 0; idy < stateNum; idy ++) {
            for(int idz = 0; idz < datas[idx].size(); idz ++)
                states[idy].probabilities[idx][idz] = 1.0 / stateNum;
        }
    }

    for(idx = 0;idx < stateNum;idx ++) {
        states[idx].gaussianTrain(gaussNum);
    }
}
double HMMSoftAutomaton::calcCost(WaveFeatureOP &input) {

    return 1.0;
}
