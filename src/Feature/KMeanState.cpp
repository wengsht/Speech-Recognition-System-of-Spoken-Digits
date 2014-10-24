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
#include <cmath>
#include "mathtool.h"

const std::pair<int, int> KMeanState::NullSeg = std::make_pair(0, -1);

KMeanState::KMeanState(std::vector<WaveFeatureOP> * templates) : HMMState(templates) {
    edgePoints.resize(templates->size());
}

KMeanState::~KMeanState() {
}
void KMeanState::gaussianTrain(int gaussianNum) {
    for(int i = 0;i < 39;i++) {
        u[i] = sigma[i] = 0;
        int cnt = 0;
        for(int j = 0;j < templates->size(); j++) {
            int kst = edgePoints[j].first, ked = edgePoints[j].second;
            for(int k = kst; k <= ked; k++) {

                u[i] += (*templates)[j][k][i];
                sigma[i] += pow((*templates)[j][k][i], 2.0);

                cnt ++;
            }
        }
        if(cnt) {
            u[i] /= cnt;
            sigma[i] /= cnt;
        }
    }

    for(int i = 0;i < 39;i++) {
        double a = sigma[i];
        sigma[i] -= (u[i] * u[i]);
    }
}

double KMeanState::nodeCost(Feature *inputFeature) {
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

