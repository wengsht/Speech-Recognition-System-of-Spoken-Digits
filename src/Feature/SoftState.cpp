// =====================================================================================
// 
//       Filename:  SoftState.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/21 21时15分06秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "SoftState.h"

SoftState::SoftState(std::vector<WaveFeatureOP> * features) : HMMState(features) {
    probabilities.resize(features->size());

    int idx;
    for(idx = 0; idx < features->size(); idx ++) {
        probabilities[idx].resize((*features)[idx].size());
    }
}

SoftState::~SoftState() {}
void SoftState::gaussianTrain(int gaussianNum) {
    puts("Gaussian Train for soft state");
}
void SoftState::nodeCost(Feature *inputFeature) {
}
