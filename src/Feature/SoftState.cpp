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

SoftState::SoftState(std::vector<WaveFeatureOP> * templates) : HMMState(templates) {
    probabilities.resize(templates->size());

    int idx;
    for(idx = 0; idx < templates->size(); idx ++) {
        probabilities[idx].resize((*templates)[idx].size());
    }
}

SoftState::~SoftState() {}

void SoftState::gaussianTrain(int gaussianNum) {
    puts("Gaussian Train for soft state");
}

double SoftState::nodeCost(Feature *inputFeature) {
    return 0.0;
}
