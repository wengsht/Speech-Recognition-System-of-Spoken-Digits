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
#include "KMeanState.h"
#include "DummyState.h"

HMMKMeanAutomaton::HMMKMeanAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum, int gaussNum, int trainTimes) : HMMAutomaton(templates, stateNum, gaussNum, trainTimes) {
}
HMMKMeanAutomaton::~HMMKMeanAutomaton() {
}

void HMMKMeanAutomaton::hmmTrain() {
    Init();

    states.clear();

    std::vector<WaveFeatureOP> & datas = *templates;

    int idx, idy;
    // 初始化 几个states
//    states.push_back(DummyState(NULL));

    for(idx = 1;idx <= stateNum;idx ++) {
        states.push_back(KMeanState(templates));
    }

    // 初始化 平均分段
    for(idx = 0; idx < datas.size(); idx++) {
        int templatesSiz = datas[idx].size();
        int stepLen = templatesSiz / stateNum;

        int startSegIdx = 0;
        int endSegIdx = 0;
        for(idy = 0; idy < stateNum; idy ++, startSegIdx += stepLen) {
            endSegIdx = startSegIdx + stepLen;
            if(idy == stateNum - 1)
                endSegIdx = templatesSiz - 1;

            // 平均分段
            states[idy].edgePoints[idx] = std::make_pair(startSegIdx, endSegIdx);
        }
    }

    for(idx = 0; idx < stateNum + 1; idx ++) {
        int nxtCnt = stateNum + 1 - idx;
        if(idx == 0) nxtCnt --;
        for(idy = idx; idy < stateNum + 1; idy ++) {
            transferCost[idx][idy] = p2cost(1.0 / nxtCnt);
        }
        // dummy -> dummy = 0
        transferCost[0][0] = Feature::IllegalDist;
    }

    for(idx = 0; idx < stateNum; idx ++) {
        states[idx].gaussianTrain(gaussNum);
    }

    //TODO
    return ;

    for(int idx = 0; idx < trainTimes; idx ++)
        if(! iterateTrain()) break;
}
double HMMKMeanAutomaton::calcCost(WaveFeatureOP &input) {
    return dtw(input, false);
}
