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

void HMMKMeanAutomaton::hmmMallocStatesAndTransfer() {
    // malloc transfer
    Init();

    clearStates();


    int idx;
    // 初始化 几个states
    states.push_back(new DummyState(templates));

    for(idx = 1;idx <= stateNum;idx ++) {
        states.push_back(new KMeanState(templates));
    }
}

void HMMKMeanAutomaton::initSegment(int templateIdx, int startI, int endI) {
    int len = endI - startI + 1;
    if(len < 0) len = 0;

    int stepLen = len / stateNum;

    int startSegIdx = startI;
    int endSegIdx = startI;

    int idy;
    for(idy = 1; idy <= stateNum; idy ++, startSegIdx += stepLen) {
        endSegIdx = startSegIdx + stepLen;
        if(idy == stateNum)
            endSegIdx = endI;

        // 平均分段
        getState(idy)->edgePoints[templateIdx] = std::make_pair(startSegIdx, endSegIdx);
    }
}

void HMMKMeanAutomaton::allEmptyStateSegment() {
    int stateIdx, templateIdx;

    for(stateIdx = 1; stateIdx <= stateNum; stateIdx++) 
        for(templateIdx = 0; templateIdx < templates->size(); templateIdx ++) 
            getState(stateIdx)->edgePoints[templateIdx] = KMeanState::NullSeg;
}

void HMMKMeanAutomaton::hmmInitilize() {
    int idx, idy;
    hmmMallocStatesAndTransfer();

    std::vector<WaveFeatureOP> & datas = *templates;

    // 初始化 平均分段
    for(idx = 0; idx < datas.size(); idx++) {
        int templateSiz = datas[idx].size();
        initSegment(idx, 0, templateSiz-1);
    }

    initTransfer();

    iterateGauss();
}

void HMMKMeanAutomaton::initTransfer() {
    int idx, idy;
    for(idx = 0; idx <= stateNum; idx++) 
        for(idy = 0; idy <= stateNum; idy++) 
            transferCost[idx][idy] = Feature::IllegalDist;

    for(idx = 1; idx <= stateNum; idx++) {
        int nxtCnt = std::min(stateNum - idx + 1, DTW_MAX_FORWARD);

        for(idy = 0; idy < nxtCnt; idy++) 
            transferCost[idx][idx+idy] = p2cost(1.0/nxtCnt);
    }

    transferCost[stateNum][stateNum] = p2cost(0.9);

    // train出允许skip的状态
    if(stateNum) {
        transferCost[0][1] = p2cost(INIT_KMEAN_0_1);
        transferCost[0][2] = p2cost(INIT_KMEAN_0_2);
    }
}

void HMMKMeanAutomaton::iterateGauss() {
    int idx;
    for(idx = 1; idx <= stateNum; idx ++) {
        states[idx]->gaussianTrain(gaussNum);
    }
}

void HMMKMeanAutomaton::hmmTrain() {
    int idx;

    hmmInitilize();

    for(idx = 0; idx < trainTimes; idx ++) {
        if(! iterateTrain()) break;
    }

    // 把train过程中开的vector什么的先释放掉
    clearTrainBuffer();
}

double HMMKMeanAutomaton::calcCost(WaveFeatureOP &input) {
    // observation 的dtw要选择sigma模式, 
    return rollDtw(input, HMMAutomaton::Sigma).second;
}

// return true if big change
bool HMMKMeanAutomaton::updateTransfer() {
    double wholeChangeCost = Feature::IllegalDist;

    for(int i = 1; i <= stateNum; i++) {
        //  处于这个state的节点个数
        int wholeCnt = 0;

        int nxtCnt[DTW_MAX_FORWARD];
        memset(nxtCnt, 0, sizeof(nxtCnt));

        // 统计每个template处于这个状态的转移
        for(int j = 0;j < templates->size(); j ++) {
            std::pair<int,int> seg = getState(i)->edgePoints[j];

            int numNode = seg.second - seg.first + 1;

            if(numNode == 0) continue;
            wholeCnt += numNode;

            nxtCnt[0] += numNode - 1;
            //                    if(seg.second == (*templates)[j].size()-1)
            //                        nxtCnt[0] += 1;

            // 统计i会转移到i+k 的node的个数
            for(int k = 1; k < DTW_MAX_FORWARD; k++) {
                int nxtState = i + k;
                if(nxtState > stateNum) break;

                if(getState(nxtState)->edgePoints[j].first - 1 == getState(i)->edgePoints[j].second) {
                    nxtCnt[k] ++;
                    // 一个template只会贡献一个转移
                    break;
                }
            }
        }

        int remain = wholeCnt;
        for(int idz = 0;idz < DTW_MAX_FORWARD;idz++) {
            if(idz+i > stateNum) break;
            remain -= nxtCnt[idz];
        }
        int addTo = i + DTW_MAX_FORWARD - 1;

        if(addTo <= stateNum)
            nxtCnt[addTo] += remain;

        if(wholeCnt == 0) {
            continue;
        }

        for(int j = 0;j < DTW_MAX_FORWARD; j++) {
            if(i+j > stateNum) break;
            double newCost = p2cost(1.0 * nxtCnt[j] / wholeCnt);

            wholeChangeCost= logInsideSum(wholeChangeCost, logInsideDist(newCost, transferCost[i][i+j]));
            //                    wholeChangeCost = fabs(newCost - transferCost[i][i+j]);

            transferCost[i][i+j] = newCost;
        }
    }

    if(stateNum >= 2) {
        int beginSkipCnt = 0;
        for(int j = 0; j < templates->size(); j++) {
            std::pair<int,int> seg = getState(1)->edgePoints[j];
            int numNode = seg.second - seg.first + 1;

            if(numNode == 0) beginSkipCnt ++;
        }

        transferCost[0][1] = p2cost(1.0 * (templates->size() - beginSkipCnt) / templates->size());
        transferCost[0][2] = p2cost(1.0 * (beginSkipCnt) / templates->size());
    }

    adjustSkippingTransfer();

    // 每个transfer有0.01的变动 TODO
    return isBigChange(wholeChangeCost);
}

// 利用path Matrix 更新这个template的分段
// 对于第idx个template，已经做了dtw，现在更新他在每个state的分段信息
void HMMKMeanAutomaton::updateSegmentation(int templateIdx, int bestFinalRowIdx) {
    int stateIdx = stateNum;
    int rowIdx = bestFinalRowIdx;

    // init
    while(stateIdx >= 1)  {
        getState(stateIdx)->edgePoints[templateIdx] = KMeanState::NullSeg;
        stateIdx --;
    }

    int featureIdx = (*templates)[templateIdx].size() - 1;

    // 分配feature到各个state
    while(featureIdx >= 0) {
        int endIdx = featureIdx;
        int startIdx = featureIdx;

        // 处于同一状态的所有节点
        while(startIdx > 0 && path[startIdx][rowIdx] == rowIdx) {
            startIdx --;
        }

        getState(rowIdx)->edgePoints[templateIdx] = std::make_pair(startIdx, endIdx);

        featureIdx = startIdx - 1;

        rowIdx = path[startIdx][rowIdx];
    }
}
