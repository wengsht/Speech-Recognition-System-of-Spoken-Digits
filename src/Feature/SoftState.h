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
#include "Gaussian.h"

class SoftState : public HMMState {
    public:
        friend class HMMSoftAutomaton;
        friend class HMMAutomaton;

        SoftState(std::vector<WaveFeatureOP> * templates);
        ~SoftState();

        void gaussianTrain(int gaussianNum);
        double nodeCost(Feature *inputFeature);

        void gaussianTrainTest(int gaussianNum);
        double nodeCostTest(Feature *inputFeature);

        void dump() {
            for(int i  =0;i < 39;i++) 
                printf("%lf %lf\n", u[i], sigma[i]);
            puts("");
        }
    private:
        // vector<WaveFeatureOP> 对应的node，处于这个state的概率
        // probabilities[i][j] 第i个wav的第j个feature属于这个state的概率
        Matrix<double> probabilities;

        void initTrain(int gaussianNum);

        void SoftTrain();
        double SoftNodeCost(Feature *f);

        std::vector<Gaussian *> GaussianSet;
        std::vector<double> weight;

        void generateInit( Feature & feature);

        void clearGaussian();

        int Point2Clusters(int templateIdx, int featureIdx);

        double totalProbability;
        double maxProbability;
        double u[39];
        double sigma[39];
};

#endif

