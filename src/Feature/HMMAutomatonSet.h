// =====================================================================================
// 
//       Filename:  HMMAutomatonSet.h
// 
//    Description:  This class is set of Automaton, derived from WaveFeatureOPSet
// 
//        Version:  0.01
//        Created:  2014/10/21 19时21分14秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMAutomatonSet_H_
#define _AUTOGUARD_HMMAutomatonSet_H_

#include "WaveFeatureOPSet.h"
#include "HMMAutomaton.h"
#include <map>
#include <string>
#include "HMMState.h"
#include <vector>

class HMMAutomatonSet : public WaveFeatureOPSet {
    READ_WRITE_DECLARE(HMMState::StateType, stateType, StateType);
    READ_WRITE_DECLARE(int, stateNum, StateNum);
    READ_WRITE_DECLARE(int, gaussNum, GaussNum);
    READ_WRITE_DECLARE(int, trainTimes, TrainTimes);
    public:
        HMMAutomatonSet(int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);
        ~HMMAutomatonSet();

        SP_RESULT train();
        std::string recognition(WaveFeatureOP & input);

        // 清空自动机，释放空间
        void clear();

    private:
        std::map< std::string, HMMAutomaton *> automatons;
};

#endif
