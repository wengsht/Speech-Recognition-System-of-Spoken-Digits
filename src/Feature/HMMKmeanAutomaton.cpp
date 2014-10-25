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

// 
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

    // 统计每个状态初始有多少个节点
    std::vector<int> nodeCnt(stateNum + 1);
    for(idx = 0; idx <= stateNum; idx++) 
        nodeCnt[idx] = 0;

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

            nodeCnt[idy] += endSegIdx - startSegIdx + 1;
        }
    }

    for(idx = 0; idx <= stateNum; idx++) 
        for(idy = 0; idy <= stateNum; idy++) 
            transferCost[idx][idy] = Feature::IllegalDist;

    for(idx = 1; idx <= stateNum; idx++) {
        int nxtCnt = std::min(stateNum - idx + 1, DTW_MAX_FORWARD);

        for(idy = 0; idy < nxtCnt; idy++) 
            transferCost[idx][idx+idy] = p2cost(1.0/nxtCnt);
    }
    /*  
    for(idx = 1; idx < stateNum; idx ++) {
        transferCost[idx][idx] = p2cost(1.0 * (nodeCnt[idx] - datas.size()) / nodeCnt[idx]);
        transferCost[idx][idx + 1] = p2cost(1.0 * datas.size() / nodeCnt[idx]);
    }

    transferCost[stateNum][stateNum] = 0.0;
    */
    // Dummy 只会走到1
    if(stateNum)
        transferCost[0][1] = 0.0;


    for(idx = 1; idx <= stateNum; idx ++) {
        states[idx]->gaussianTrain(gaussNum);
    }

    /*  
    for(idx = 0;idx <= stateNum; idx++) {
        for(idy = 0; idy <= stateNum; idy++) 
            printf("%lf ", transferCost[idx][idy]);
        puts("");
    }
    */
    for(idx = 0; idx < trainTimes; idx ++) {
//        printf("%d\n", idx);
        if(! iterateTrain()) break;
    }


    // 把train过程中开的vector什么的先释放掉
    clearTrainBuffer();
}

double HMMKMeanAutomaton::calcCost(WaveFeatureOP &input) {
    // observation 的dtw要选择sigma模式, 
    return rollDtw(input, HMMAutomaton::Sigma).second;
}
