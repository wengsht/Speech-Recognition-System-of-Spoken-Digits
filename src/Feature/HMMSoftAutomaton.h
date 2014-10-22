// =====================================================================================
// 
//       Filename:  HMMSoftAutomaton.h
// 
//    Description:  This is Soft Automaton derived from HMMAutomaton
// 
//        Version:  0.01
//        Created:  2014/10/21 22时31分35秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMSoftAutomaton_H_
#define _AUTOGUARD_HMMSoftAutomaton_H_

#include <vector>
#include "WaveFeatureOP.h"
#include "configure_hmm.h"
#include "HMMAutomaton.h"
#include "SoftState.h"

class HMMSoftAutomaton : public HMMAutomaton {
    public:
        HMMSoftAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);
        ~HMMSoftAutomaton();

        void hmmTrain();
        double calcCost(WaveFeatureOP &input);
    private:
        std::vector<SoftState> states;
};

#endif

