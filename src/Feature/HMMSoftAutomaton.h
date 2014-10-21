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
        HMMSoftAutomaton(std::vector<WaveFeatureOP> *features, int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM);
        ~HMMSoftAutomaton();

        void hmmTrain();
    private:
        std::vector<SoftState> states;
};

#endif

