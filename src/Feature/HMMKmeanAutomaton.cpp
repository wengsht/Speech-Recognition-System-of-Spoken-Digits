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

    clearStates();

    std::vector<WaveFeatureOP> & datas = *templates;

    int idx, idy;
    // 初始化 几个states
//    states.push_back(DummyState(NULL));

    states.push_back(new DummyState(templates));

    for(idx = 1;idx <= stateNum;idx ++) {
        states.push_back(new KMeanState(templates));
    }

    // 初始化 平均分段
    for(idx = 0; idx < datas.size(); idx++) {
        int templatesSiz = datas[idx].size();
        int stepLen = templatesSiz / stateNum;

        int startSegIdx = 0;
        int endSegIdx = 0;
        for(idy = 1; idy <= stateNum; idy ++, startSegIdx += stepLen) {
            endSegIdx = startSegIdx + stepLen;
            if(idy == stateNum)
                endSegIdx = templatesSiz - 1;

            // 平均分段
            getState(idy)->edgePoints[idx] = std::make_pair(startSegIdx, endSegIdx);
        }
    }

    for(idx = 0; idx < stateNum + 1; idx ++) {
        int forwardNum = std::min(DTW_MAX_FORWARD, stateNum - idx + 1);

        for(idy = 0; idy < forwardNum && idx + idy <= stateNum; idy++) 
                transferCost[idx][idx+idy] = p2cost(1.0 / forwardNum);
    }
    // Dummy 只会走到1
    int forwardNum = std::min(DTW_MAX_FORWARD, stateNum + 1);
    for(idy = 0; idy < forwardNum; idy++) 
        transferCost[0][0+idy] = Feature::IllegalDist;
    if(stateNum)
        transferCost[0][1] = 0.0;

    for(idx = 1; idx <= stateNum; idx ++) {
        states[idx]->gaussianTrain(gaussNum);
    }

    for(idx = 0; idx < trainTimes; idx ++)
        if(! iterateTrain()) break;

    // 把train过程中开的vector什么的先释放掉
    clearTrainBuffer();
}

double HMMKMeanAutomaton::calcCost(WaveFeatureOP &input) {
    // observation 的dtw要选择sigma模式, 
    return rollDtw(input, HMMAutomaton::Sigma).second;
}
