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
#include <cmath>

SoftState::SoftState(std::vector<WaveFeatureOP> * templates) : HMMState(templates) {
    probabilities.resize(templates->size());

    int idx;
    for(idx = 0; idx < templates->size(); idx ++) {
        probabilities[idx].resize((*templates)[idx].size());
    }
}

SoftState::~SoftState() {}

void SoftState::gaussianTrain(int gaussianNum) {
    for(int i = 0;i < 39;i++) {
        u[i] = sigma[i] = 0;
        double cnt = 0;
        for(int j = 0;j < templates->size(); j++) {
            for(int k = 0; k < (*templates)[j].size(); k++) {
                u[i] += (*templates)[j][k][i] * probabilities[j][k];
                sigma[i] += probabilities[j][k] * pow((*templates)[j][k][i], 2.0);

                cnt += probabilities[j][k];
            }
        }
        if(cnt > 0.0) {
            u[i] /= cnt;
            sigma[i] /= cnt;
        }
    }

    for(int i = 0;i < 39;i++) {
        sigma[i] -= (u[i] * u[i]);
    }
}

double SoftState::nodeCost(Feature *inputFeature) {
    if(u[0] == 0.0)
        return Feature::IllegalDist;

    double a = 0.0, b = 0.0;
    for(int i = 0;i < 39;i++) {
        a += log(2*3.1415926*sigma[i]);

        b += pow((*inputFeature)[i] - u[i], 2.0) / sigma[i];
    }
    a *= 0.5;
    b *= 0.5;

    return a+b; //p2cost(a+b);
}
