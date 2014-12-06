// =====================================================================================
// 
//       Filename:  HMMRecognition.h
// 
//    Description:  This class is the front end of HMM Recognition 
//
//    Please ! when using it, call close() before you destroy an object
//                  
// 
//        Version:  0.01
//        Created:  2014/10/21 19时19分15秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMRecognition_H_
#define _AUTOGUARD_HMMRecognition_H_

#include "HMMAutomatonSet.h"
#include "HMMState.h"
#include "configure_basic.h"
#include "configure.h"
#include "Feature.h"
#include <vector>
#include <string>
#include <fstream>
using namespace std;

class HMMRecognition {
    // KMEAN / SOFT
    READ_WRITE_DECLARE(HMMState::StateType, stateType, StateType);

    public:
        HMMRecognition(int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);
        virtual ~HMMRecognition();

        // add mfcc features into templates set
        SP_RESULT loadTemplates(char *templateDir, WaveFeatureOP::LOAD_TYPE loadType = WaveFeatureOP::FULL_LOAD);

        // hmm Train 
        SP_RESULT hmmTrain(HMMAutomaton::TRAIN_TYPE type = HMMAutomaton::SEG);

        SP_RESULT hmmTryLoad(char *templateDir);;

        // 返回recognition的单词
        //
        virtual std::string hmmRecognition(WaveFeatureOP & input);

        // You should call this function before you destroy this object!!
        virtual void close();

        void setStateNum(int stateNum) {
            this->stateNum = stateNum;

            automatons.setStateNum(stateNum);
        }

        void setGaussNum(int gaussNum) {
            this->gaussNum = gaussNum;

            automatons.setGaussNum(gaussNum);
        }

        void setTrainTimes(int trainTimes) {
            this->trainTimes = trainTimes;

            automatons.setTrainTimes(trainTimes);
        }

        // 
        void dumpAutomaton(std::ostream & out);
    protected:
        bool loadHMMModel(HMMAutomatonSet & automatons, char * suffix = "");
        void storeHMMModel();
        string generateHMMFileName(char *suffix = "");

        int stateNum;
        int gaussNum;
        int trainTimes;

        string templateDir;

        HMMAutomatonSet automatons;
};

#endif
