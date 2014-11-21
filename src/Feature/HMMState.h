// =====================================================================================
// 
//       Filename:  HMMState.h
// 
//    Description:  This is virtual class for HMM state
// 
//        Version:  0.01
//        Created:  2014/10/21 20时41分20秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMState_H_
#define _AUTOGUARD_HMMState_H_

#include <vector>
#include "WaveFeatureOP.h"
#include "Feature.h"
#include <sstream>

class DummyState;

class HMMState {
    public:
        enum StateType {
            DUMMY,
            KMEAN,
            SOFT,
            UNEMIT
        };
    public:
        friend class HMMAutomaton;

        HMMState(std::vector<WaveFeatureOP> *templates) : templates(templates) {}
        HMMState() : templates(NULL) {}
        virtual ~HMMState() {};

        virtual void gaussianTrain(int gaussianNum) = 0;
        virtual double nodeCost(Feature *inputFeature) = 0;

        virtual void setTemplates(std::vector<WaveFeatureOP> * newTemps) {
        }
        virtual void load(std::stringstream &in, int &gaussNum) = 0;
        virtual void store(std::stringstream &out) = 0;

    protected:
        std::vector<WaveFeatureOP> * templates;
};

#endif

