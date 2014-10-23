// =====================================================================================
// 
//       Filename:  DummyState.h
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/23 00时12分24秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_DummyState_H_
#define _AUTOGUARD_DummyState_H_

#include "HMMState.h"
#include "Feature.h"
#include "WaveFeatureOP.h"
class DummyState : public HMMState {
    public:
        DummyState(std::vector<WaveFeatureOP> *templates) : HMMState(templates) {}
        ~DummyState() {}

        void gaussianTrain(int gaussianNum) {
        }
        double nodeCost(Feature *inputFeature) {
            return Feature::IllegalDist;
        }

    private:
};

#endif

