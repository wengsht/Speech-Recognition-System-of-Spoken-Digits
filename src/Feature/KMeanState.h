// =====================================================================================
// 
//       Filename:  KMeanState.h
// 
//    Description:  Automaton state for KMean
// 
//        Version:  0.01
//        Created:  2014/10/21 20时42分38秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_KMeanState_H_
#define _AUTOGUARD_KMeanState_H_

#include "HMMState.h"
#include <vector> 

class KMeanState : public HMMState {
    public:
        friend class HMMKMeanAutomaton;
        friend class HMMAutomaton;

        KMeanState(std::vector<WaveFeatureOP> * templates);

        ~KMeanState();

        void gaussianTrain(int gaussianNum);

        double nodeProbability(Feature *inputFeature);


    private:
        // 对于Kmean， 存储的时候属于这个state的线段 
        // 注意是和基类的vector<WaveFeatureOP> 一一对应的
        std::vector< std::pair<int, int> > edgePoints;
};

#endif

