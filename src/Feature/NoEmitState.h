// =====================================================================================
// 
//       Filename:  NoEmitState.h
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/19 01时32分39秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_NoEmitState_H_
#define _AUTOGUARD_NoEmitState_H_

#include "HMMState.h"

class NoEmitState : public HMMState {
    public:
        NoEmitState() {}
        ~NoEmitState() {}

        void gaussianTrain(int gaussianNum) {}
        double nodeCost(Feature *inputFeature) { 
            return 0;
        }

        void load(std::stringstream &in, int gaussNum) {}
        void store(std::stringstream &out){}
    private:
};

#endif

