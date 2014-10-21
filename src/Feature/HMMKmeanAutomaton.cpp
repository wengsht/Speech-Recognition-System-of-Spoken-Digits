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

#include "HMMKMeanAutomaton.h"

HMMKMeanAutomaton::HMMKMeanAutomaton(std::vector<WaveFeatureOP> *features, int stateNum, int gaussNum) : HMMAutomaton(features, stateNum, gaussNum) {
}
HMMKMeanAutomaton::~HMMKMeanAutomaton() {
}

void HMMKMeanAutomaton::hmmTrain() {
    states.clear();

    std::vector<WaveFeatureOP> & datas = *features;

    int idx, idy;
    // 初始化 几个states
    for(idx = 0;idx < stateNum;idx ++) {
        states.push_back(KMeanState(features));
    }

    // 初始化 平均分段
    for(idx = 0; idx < datas.size(); idx++) {
        int featureSiz = datas[idx].size();
        int stepLen = featureSiz / stateNum;

        int startSegIdx = 0;
        int endSegIdx = 0;
        for(idy = 0; idy < stateNum; idy ++, startSegIdx += stepLen) {
            endSegIdx = startSegIdx + stepLen;
            if(idy == stateNum - 1)
                endSegIdx = featureSiz - 1;

            states[idy].edgePoints[idx] = std::make_pair(startSegIdx, endSegIdx);
        }
    }

    for(idx = 0; idx < stateNum; idx ++) {
        states[idx].gaussianTrain(gaussNum);
    }

    // TODO iterate the training
}
