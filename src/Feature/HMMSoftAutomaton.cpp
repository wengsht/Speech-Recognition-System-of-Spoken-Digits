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
#include "DummyState.h"

HMMSoftAutomaton::HMMSoftAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum, int gaussNum, int trainTimes) : HMMAutomaton(templates, stateNum, gaussNum, trainTimes) {
}
HMMSoftAutomaton::~HMMSoftAutomaton() {
}

void HMMSoftAutomaton::hmmTrain() {
    Init();

    clearStates();

    std::vector<WaveFeatureOP> & datas = *templates;

//    states.push_back(DummyState(NULL));
    int idx, idy, idz;

    states.push_back(new DummyState(NULL));
    // 初始化 几个states
    for(idx = 1;idx <= stateNum;idx ++) {
        states.push_back(new SoftState(templates));
    }

    // 统计每个状态初始有多少个节点
    std::vector<int> nodeCnt(stateNum + 1);
    for(idx = 0; idx <= stateNum; idx++) 
        nodeCnt[idx] = 0;

    for(idx = 0; idx < datas.size();idx ++) 
        for(idy = 1; idy <=stateNum; idy ++)
            for(idz = 0; idz < datas[idx].size(); idz ++) 
                getState(idy)->probabilities[idx][idz] = 0.0;

    //  初始化， 仍然平均分段
    for(idx = 0; idx < datas.size(); idx++) {
        int templateSiz = datas[idx].size();
        int featurePerState = templateSiz / stateNum;

        for(idy = 1, idz = 0; idy <= stateNum; idy ++) {
            if(idy == stateNum) featurePerState =  templateSiz - idz;

            int itr;
            for(itr = 0; itr < featurePerState; itr ++) {
                getState(idy)->probabilities[idx][idz+itr] = 1.0;

                nodeCnt[idy] ++;
            }
            idz += itr;

        }
    }

    for(idx = 1;idx <= stateNum;idx ++) {
        getState(idx)->gaussianTrain(gaussNum);

//        printf("%d\n", idx);
//        getState(idx)->dump();
    }

    // init transferCost
    for(idx = 0; idx <= stateNum; idx++) 
        for(idy = 0; idy <= stateNum; idy++) 
            transferCost[idx][idy] = Feature::IllegalDist;

    for(idx = 1; idx < stateNum; idx ++) {
        transferCost[idx][idx] = p2cost(1.0 * (nodeCnt[idx] - datas.size()) / nodeCnt[idx]);
        transferCost[idx][idx + 1] = p2cost(1.0 * datas.size() / nodeCnt[idx]);
    }

    transferCost[stateNum][stateNum] = 0.0;

    /*  
    for(idx = 0; idx <= stateNum; idx++) {
        for(idy = 0; idy <= stateNum; idy++)  
            printf("%lf ", transferCost[idx][idy]);
        puts("");
    }
    */


    // Dummy 只会走到1
    if(stateNum)
        transferCost[0][1] = 0.0;


    for(idx = 0; idx < trainTimes; idx++)  {
        if(! iterateTrain()) break;
    }

    clearTrainBuffer();
}
double HMMSoftAutomaton::calcCost(WaveFeatureOP &input) {
    // observation 的dtw要选择sigma模式, 
    return rollDtw(input, HMMAutomaton::Sigma).second;
}

void HMMSoftAutomaton::beforeAlphaBeta(WaveFeatureOP & features) {
    if(features.size() <= 0 || stateNum <= 0)
        return ;

    int T = features.size();
    int stateIdx;
    // nodeCostTmp[s][t] buffer第s个state第t个节点的nodeCost

    if(nodeCostTmp.size() < stateNum + 1) {
        nodeCostTmp.resize(stateNum + 1);
    }

    for(stateIdx = 1; stateIdx <= stateNum; stateIdx ++)
        if(nodeCostTmp[stateIdx].size() < T)
            nodeCostTmp[stateIdx].resize(T);

    for(stateIdx = 1; stateIdx <= stateNum; stateIdx ++) 
        for(int tIdx = 0; tIdx < T; tIdx++) 
            nodeCostTmp[stateIdx][tIdx] = getState(stateIdx)->nodeCost(&features[tIdx]);

//    if(nodeCostTmp[1][0] < Feature::IllegalDist)
//        printf("%lf\n", nodeCostTmp[1][0]);

    int idx, idy;
    if(alphaCost.size() < stateNum + 1) {
        alphaCost.resize(stateNum + 1);
    }
    for(idx = 0;idx <= stateNum ; idx++) 
        if(alphaCost[idx].size() < T)
            alphaCost[idx].resize(T);

    if(betaCost.size() < stateNum + 1) {
        betaCost.resize(stateNum + 1);
    }

    for(idx = 0;idx <= stateNum; idx++) 
        if(betaCost[idx].size() < T)
            betaCost[idx].resize(T);

    // alpha: 第0个feature处于第一个状态 
    // beta: 最后一帧都是0.0
    for(idx = 0; idx <= stateNum; idx++) {
        alphaCost[idx][0] = Feature::IllegalDist;
        betaCost[idx][T-1] = 0.0;
    }
    alphaCost[1][0] = nodeCostTmp[1][0]; //getState(1)->nodeCost(&(features[0]));

    // 处于0状态非法
    betaCost[0][T-1] = Feature::IllegalDist;
}

void HMMSoftAutomaton::calcAlphaBeta(WaveFeatureOP & features) {
    int T = features.size();
    if(T <= 0 || stateNum <= 0)
        return ;

    beforeAlphaBeta(features);

    int alphaIdx, betaIdx, alphaStatePreIdx, betaStateNxtIdx;
    int stateIdx, dtwIdx;

    // 迭代alpha 和 beta， 并行
    for(alphaIdx = 1; alphaIdx < T; alphaIdx ++) {
        betaIdx = T - alphaIdx - 1;
        
        for(stateIdx = 1; stateIdx <= stateNum; stateIdx ++) {
            // alpha = alpha * p[i][i] + nodeCost[i][j]
            //
            alphaCost[stateIdx][alphaIdx] = alphaCost[stateIdx][alphaIdx - 1] + \
                                            transferCost[stateIdx][stateIdx] + \
                                            nodeCostTmp[stateIdx][alphaIdx];

            // beta = beta * p[i][i] * nodeCost[i][j+1]
            betaCost[stateIdx][betaIdx] = betaCost[stateIdx][betaIdx+1] + \
                                          transferCost[stateIdx][stateIdx] + \
                                          nodeCostTmp[stateIdx][betaIdx + 1];

            for(dtwIdx = 1; dtwIdx < DTW_MAX_FORWARD; dtwIdx ++) {
                alphaStatePreIdx = stateIdx - dtwIdx;
                betaStateNxtIdx  = stateIdx + dtwIdx;

                if(alphaStatePreIdx >= 1) {
                    alphaCost[stateIdx][alphaIdx] = logInsideSum(alphaCost[stateIdx][alphaIdx], \
                            alphaCost[alphaStatePreIdx][alphaIdx - 1] + \
                            transferCost[alphaStatePreIdx][stateIdx] + \
                            nodeCostTmp[stateIdx][alphaIdx]);
                }
                if(betaStateNxtIdx <= stateNum) {
                    betaCost[stateIdx][betaIdx] = logInsideSum(betaCost[stateIdx][betaIdx], \
                            betaCost[betaStateNxtIdx][betaIdx+1] + \
                            transferCost[stateIdx][betaStateNxtIdx] + \
                            nodeCostTmp[betaStateNxtIdx][betaIdx + 1]);
                }
            }
        }
    }
}

void HMMSoftAutomaton::initIterate() {
    int idx, idy;
    if(YustCost.size() < stateNum + 1) 
        YustCost.resize(stateNum + 1);

    if(Ys2sNxtCost.size() < stateNum + 1) {
        Ys2sNxtCost.resize(stateNum + 1);
    }
    for(idx = 0;idx <= stateNum; idx ++) 
        Ys2sNxtCost[idx].resize(stateNum + 1);

    for(idx = 0; idx <= stateNum; idx ++) {
        YustCost[idx] = Feature::IllegalDist;

        for(idy = 0; idy < DTW_MAX_FORWARD; idy ++) {
            if(idx + idy <= stateNum)
                Ys2sNxtCost[idx][idx + idy] = Feature::IllegalDist;
        }
    }
}

bool HMMSoftAutomaton::updateTransfer() {
    int stateIdx1, stateIdx2, dtwIdx;

    double totalChange, tmp;
    for(stateIdx1 = 1; stateIdx1 <= stateNum; stateIdx1++) {
        for(dtwIdx = 0; dtwIdx < DTW_MAX_FORWARD; dtwIdx++) {
            stateIdx2 = stateIdx1 + dtwIdx;
            if(stateIdx2 > stateNum) break;

            tmp = transferCost[stateIdx1][stateIdx2];
            transferCost[stateIdx1][stateIdx2] = Ys2sNxtCost[stateIdx1][stateIdx2] - YustCost[stateIdx1];

            totalChange += fabs(tmp - transferCost[stateIdx1][stateIdx2]);
        }
    }

    return isBigChange(totalChange);
}

void HMMSoftAutomaton::updateTemplateNode(int templateIdx) {
    int stateIdx, tIdx;
    int T = (*templates)[templateIdx].size();
    double tmpProb;

    std::vector<double> tmpCost;
    tmpCost.resize(stateNum + 1);

    for(tIdx = 0; tIdx < T; tIdx ++) {
        double sigmaCost = Feature::IllegalDist;
        for(stateIdx = 1; stateIdx <= stateNum; stateIdx++) {
            tmpCost[stateIdx] = alphaCost[stateIdx][tIdx] + betaCost[stateIdx][tIdx];

            sigmaCost = logInsideSum(sigmaCost, tmpCost[stateIdx]);
        }

        for(stateIdx = 1; stateIdx <= stateNum; stateIdx++) {
            tmpCost[stateIdx] -= sigmaCost;

            tmpProb = cost2p(tmpCost[stateIdx]);

            getState(stateIdx)->probabilities[templateIdx][tIdx] = tmpProb;

            YustCost[stateIdx] = logInsideSum(YustCost[stateIdx], tmpCost[stateIdx]);
        }
    }

}

void HMMSoftAutomaton::updateTemplateTransfer(int templateIdx) {
    int stateIdx1, stateIdx2, tIdx, dtwIdx, idx;

    int T = (*templates)[templateIdx].size();

    double tmpTotalCost;
    Matrix<double> s2sNxtCost;
    s2sNxtCost.resize(stateNum + 1);
    for(idx = 1; idx <= stateNum; idx++) 
        s2sNxtCost[idx].resize(stateNum + 1);

    for(tIdx = 0; tIdx < T - 1; tIdx ++) {
        tmpTotalCost = Feature::IllegalDist;
        for(stateIdx1 = 1; stateIdx1 <= stateNum; stateIdx1++) {
            // 只需要算dtw 3步 的转移， 
            for(dtwIdx = 0; dtwIdx < DTW_MAX_FORWARD; dtwIdx++) {
                stateIdx2 = stateIdx1 + dtwIdx;
                if(stateIdx2 > stateNum)
                    break;

                s2sNxtCost[stateIdx1][stateIdx2] = alphaCost[stateIdx1][tIdx] + transferCost[stateIdx1][stateIdx2] + nodeCostTmp[stateIdx2][tIdx + 1] + betaCost[stateIdx2][tIdx + 1];

                tmpTotalCost = logInsideSum(tmpTotalCost, s2sNxtCost[stateIdx1][stateIdx2]);
            }
        }
        // 把值累加到全局的s->sNxt 的转移概率中
        
        for(stateIdx1 = 1; stateIdx1 <= stateNum; stateIdx1++) {
            // 只需要算dtw 3步 的转移， 
            for(dtwIdx = 0; dtwIdx < DTW_MAX_FORWARD; dtwIdx++) {
                stateIdx2 = stateIdx1 + dtwIdx;
                if(stateIdx2 > stateNum)
                    break;

                double tmpCost = s2sNxtCost[stateIdx1][stateIdx2] - tmpTotalCost;

                Ys2sNxtCost[stateIdx1][stateIdx2] = logInsideSum(Ys2sNxtCost[stateIdx1][stateIdx2], tmpCost);
            }
        }
    }
}

bool HMMSoftAutomaton::iterateTrain() {
    int idx, idy;
    initIterate();

    // 对所有的template做迭代
    for(idx = 0; idx < templates->size(); idx++) {
        calcAlphaBeta((*templates)[idx]);

        // 1. 根据alpha 和 beta的缓存更新template的feature在各个状态的分布
        updateTemplateNode(idx);

        // 2. 更新计算s->sNxt , 累加而已
        updateTemplateTransfer(idx);
    }

    bool bigChange = updateTransfer();

    /*  
    for(int i = 1;i <= stateNum; i++) {
        for(int j = 1;j <= stateNum; j++) {
            printf("%lf ", transferCost[i][j]);
        }
        puts("");
    }
*/
    for(idx = 1; idx <= stateNum; idx++) {
        getState(idx)->gaussianTrain(gaussNum);
    }
    return bigChange;
}
