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

class HMMAutomaton {
    READ_WRITE_DECLARE(int , stateNum, StateNum);
    READ_WRITE_DECLARE(int , gaussNum, GaussNum);
public:
    HMMAutomaton(std::vector<WaveFeatureOP> *feature, int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM);

    virtual ~HMMAutomaton() {};
    virtual void hmmTrain() = 0;
    
protected:
    // WaveFeatureOP 存储一个template/input, 
    // use waveFeatureOP.size() and waveFeatureOP[idx] to iterate each wav's features
    // You Should ensure this pointer point to existed object during computing!!!
    std::vector<WaveFeatureOP> * features;

    // states之间转移概率
    Matrix<double> transferProbability;
};
#endif
