// =====================================================================================
// 
//       Filename:  KMeanState.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/21 21时13分32秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "KMeanState.h"

KMeanState::KMeanState(std::vector<WaveFeatureOP> * features) : HMMState(features) {
    edgePoints.resize(features->size());
}

KMeanState::~KMeanState() {}
void KMeanState::gaussianTrain(int gaussianNum) {
    puts("Gaussian Train for kmean state");
}

void KMeanState::nodeCost(Feature *inputFeature) {
}

