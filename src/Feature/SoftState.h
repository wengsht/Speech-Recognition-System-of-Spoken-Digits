// =====================================================================================
// 
//       Filename:  SoftState.h
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/21 20时57分31秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_SoftState_H_
#define _AUTOGUARD_SoftState_H_

#include "HMMState.h"
#include "configure_basic.h"
#include "WaveFeatureOP.h"

class SoftState : public HMMState {
    public:
        friend class HMMSoftAutomaton;
        friend class HMMAutomaton;
        SoftState(std::vector<WaveFeatureOP> * templates);
        ~SoftState();

        void gaussianTrain(int gaussianNum);
        double nodeCost(Feature *inputFeature);

    private:
        // vector<WaveFeatureOP> 对应的node，处于这个state的概率
        // probabilities[i][j] 第i个wav的第j个feature属于这个state的概率
        Matrix<double> probabilities;
};

#endif

