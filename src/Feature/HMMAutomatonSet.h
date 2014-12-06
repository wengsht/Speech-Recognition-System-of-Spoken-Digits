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
// =====================================================================================
#ifndef _AUTOGUARD_HMMAutomatonSet_H_
#define _AUTOGUARD_HMMAutomatonSet_H_

#include "WaveFeatureOPSet.h"
#include "HMMAutomaton.h"
#include <map>
#include <string>
#include "HMMState.h"
#include <vector>
#include <fstream>
#include <sstream>

class HMMAutomatonSet : public WaveFeatureOPSet {
    READ_WRITE_DECLARE(HMMState::StateType, stateType, StateType);
    READ_WRITE_DECLARE(int, stateNum, StateNum);
    READ_WRITE_DECLARE(int, gaussNum, GaussNum);
    READ_WRITE_DECLARE(int, trainTimes, TrainTimes);
    public:
        HMMAutomatonSet(int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);

        ~HMMAutomatonSet();

        SP_RESULT train(HMMAutomaton::TRAIN_TYPE type = HMMAutomaton::SEG);
        std::string recognition(WaveFeatureOP & input);

        // 清空自动机，释放空间
        void clear();

        void dumpAutomaton(std::ostream & out);

        virtual void hmmFirstItr() {

        }

    private:
        SP_RESULT segTrain();
        SP_RESULT seqTrain();

        static void hmmTrainTask(void *in);

        // 新建自动机， 只有需要train的时候才调用此函数
        // load 不需要
        void reGenerateAutomaton();
        int getSpecificStateNum( std::string &word );
        void loadSpecStateNum();
    public:
        void load(std::ifstream &in);
        void store(std::ofstream &out);

        std::map< std::string, HMMAutomaton *> & getAutomatons() {
            return automatons;
        }

    private:
        std::map< std::string, HMMAutomaton *> automatons;

        HMMAutomatonSet * initAutomatonSet;

        std::vector< WaveFeatureOP > mixedWavs;

        std::map< std::string, int> specStateNums;
};

#endif
