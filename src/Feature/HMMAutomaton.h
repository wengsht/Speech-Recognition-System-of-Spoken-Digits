// =====================================================================================
// 
//       Filename:  HMMAutomaton.h
// 
//    Description:  This class store a automaton
// 
//        Version:  0.01
//        Created:  2014/10/21 19时40分17秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMAutomaton_H_
#define _AUTOGUARD_HMMAutomaton_H_

#include "WaveFeatureOP.h"
#include "configure_basic.h"
#include "mathtool.h"

#include <vector>
#include "tool.h"
#include "configure_hmm.h"
#include "HMMState.h"
#include "math.h"
#include <sstream>

class HMMAutomaton {
    READ_WRITE_DECLARE(int , stateNum, StateNum);
    READ_WRITE_DECLARE(int , gaussNum, GaussNum);
    READ_WRITE_DECLARE(int , trainTimes, TrainTimes);
public:
    friend class SeqModel;
    enum TRAIN_TYPE {
        SEG,
        SEQ
    };

    HMMAutomaton(std::vector<WaveFeatureOP> *feature, int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);

    virtual ~HMMAutomaton() {};
    virtual void hmmTrain() = 0;
    virtual double calcCost(WaveFeatureOP &input) = 0;

    //  you should call this before you destroy this object
    void close();

    // 
    // give skipping a floor probability
    //
    void adjustSkippingTransfer();

    virtual void load(std::stringstream &in) = 0;
    void store(std::stringstream &out);
    void dumpTransfer(std::ostream & out);

    // return probability that stateID transfer to dummy ending

    double enddingProbability(int stateID);
protected:
    enum dtwType {
        Maximum, 
        Sigma
    };

    Matrix<int> path;
    std::vector<double> rollColumnCost[2];

    bool isBigChange(double totalChangeCost) {
        return cost2p(totalChangeCost) >= 0.005 * stateNum * DTW_MAX_FORWARD;
    }
    // dtw 初始化第-1列
    void rollDtwInit(WaveFeatureOP &features, dtwType type) {
        int idx;
        for(idx = 0; idx < 2; idx ++)
            if(rollColumnCost[idx].size() < stateNum+1)
                rollColumnCost[idx].resize(stateNum + 1);

        if(path.size() < features.size()) {
            path.resize(features.size());

            for(idx = 0; idx < path.size(); idx ++)
                path[idx].resize(stateNum + 1);
        }
        int columnIdx = -1;
        int rollIdx = getRollIdx(columnIdx);

        for(idx = 0; idx <= stateNum; idx++) {
            rollColumnCost[rollIdx][idx] = Feature::IllegalDist;
            rollColumnCost[rollIdx ^ 1][idx] = Feature::IllegalDist;
        }

        rollColumnCost[rollIdx][0] = transferCost[0][1];
        rollColumnCost[rollIdx][2] = transferCost[0][2];

    }

    // 滚动数组dtw
    // return final state and cost
    std::pair<int, double> rollDtw(WaveFeatureOP & features, dtwType type) {
        rollDtwInit(features, type);

        int columnIdx, rollIdx, preIdx, stateIdx;

        int resIdx = -1;
        double resValue = 0.0;

        for(columnIdx = 0; columnIdx < features.size(); columnIdx ++) {
            rollIdx = getRollIdx(columnIdx);
            preIdx  = rollIdx ^ 1;

            for(stateIdx = 1; stateIdx <= stateNum; stateIdx ++) {
                double nodeCost = states[stateIdx]->nodeCost(&features[columnIdx]);

                rollColumnCost[rollIdx][stateIdx] = rollColumnCost[preIdx][stateIdx] + transferCost[stateIdx][stateIdx] + nodeCost;

                // should record the path 
                if(type == Maximum) {
                    path[columnIdx][stateIdx] = stateIdx;
                }

                for(int i = 1; i < DTW_MAX_FORWARD;i++) {
                    int preStateIdx = stateIdx - i;

                    if(preStateIdx < 0) break;

                    double mayPathCost = rollColumnCost[preIdx][preStateIdx] + transferCost[preStateIdx][stateIdx] + nodeCost;

                    //                if(stateIdx == 1 && type == Maximum && columnIdx == 0) 
                    //                    printf("%lf\n", nodeCost);

                    //                    printf("%lf %lf\n", mayPathCost, rollColumnCost[rollIdx][stateIdx]);
                    //                    printf("%lf\n", rollColumnCost[rollIdx][stateIdx]);
                    // should record the path 
                    if(type == Maximum) {
                        if(Feature::better(mayPathCost, rollColumnCost[rollIdx][stateIdx])) {
                            rollColumnCost[rollIdx][stateIdx] = mayPathCost;

                            path[columnIdx][stateIdx] = preStateIdx;
                        }
                    }
                    // need not to record the path
                    else if(type == Sigma) {
                        rollColumnCost[rollIdx][stateIdx] = logInsideSum(rollColumnCost[rollIdx][stateIdx], mayPathCost);
                    }
                }
            }

            // 处于0的cost相当高= =
            rollColumnCost[rollIdx][0] = Feature::IllegalDist;
        }

        columnIdx = features.size() - 1;
        rollIdx = getRollIdx(columnIdx);

        resValue = rollColumnCost[rollIdx][1];
        resIdx   = 1;

        for(int i = 2; i <= stateNum; i++) {
            if(type == Maximum) {
                if(Feature::better(rollColumnCost[rollIdx][i], resValue)) {
                    resValue = rollColumnCost[rollIdx][i];

                    resIdx = i;
                }
            }
            else if(type == Sigma) {
                resValue = logInsideSum(resValue, rollColumnCost[rollIdx][i]);
            }
        }

        return std::make_pair(resIdx, resValue);
    }
    // 释放自动机中的状态的空间
    void clearStates();
    // 滚动数组的下标
    inline int getRollIdx(int columnIdx) {
        return columnIdx & 1;
    }
    void Init() {
        transferCost.resize(stateNum + 1);

        // i = 0 ==> dummy state
        for(int i = 0;i < stateNum + 1; i++) {
            transferCost[i].resize(stateNum + 1);
        }
    }
    // WaveFeatureOP 存储一个template/input, 
    // use waveFeatureOP.size() and waveFeatureOP[idx] to iterate each wav's features
    // You Should ensure this pointer point to existed object during computing!!!
    std::vector<WaveFeatureOP> * templates;

    // states之间转移概率
    // 0 = dummy state
    Matrix<double> transferCost;

    std::vector<HMMState *> states;
};
#endif
