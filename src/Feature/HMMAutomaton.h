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

#include <vector>
#include "tool.h"
#include "configure_hmm.h"
#include "HMMState.h"
#include "math.h"

class HMMAutomaton {
    READ_WRITE_DECLARE(int , stateNum, StateNum);
    READ_WRITE_DECLARE(int , gaussNum, GaussNum);
    READ_WRITE_DECLARE(int , trainTimes, TrainTimes);
public:
    HMMAutomaton(std::vector<WaveFeatureOP> *feature, int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);

    virtual ~HMMAutomaton() {};
    virtual void hmmTrain() = 0;
    virtual double calcCost(WaveFeatureOP &input) = 0;
    
protected:
    // probability  to cost
    double p2cost(double p) {
        return - log(p);
    }
    // 因为自动机存储的时候从0开始。。又有个dummy 
    inline int getStateIdx(int idx) {
        return idx - 1;
    }
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

        rollColumnCost[0].resize(stateNum + 1);
        rollColumnCost[1].resize(stateNum + 1);
    }
    //  记录路径的dtw
    void recordInit(WaveFeatureOP &input) {
        if(path.size() >= input.size())
            return ;
        path.resize(input.size());
        for(int i = 0;i < path.size(); i++) 
            path[i].resize(stateNum + 1);
    }
    // WaveFeatureOP 存储一个template/input, 
    // use waveFeatureOP.size() and waveFeatureOP[idx] to iterate each wav's features
    // You Should ensure this pointer point to existed object during computing!!!
    std::vector<WaveFeatureOP> * templates;

    // states之间转移概率
    // 0 = dummy state
    Matrix<double> transferCost;
    Matrix<int> path;
    std::vector<double> rollColumnCost[2];


};
#endif
