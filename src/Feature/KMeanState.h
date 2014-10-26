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
#include "Gaussian.h"
class KMeanState : public HMMState {
    public:
        const static std::pair<int, int> NullSeg ;
        friend class HMMKMeanAutomaton;
        friend class HMMAutomaton;

        KMeanState(std::vector<WaveFeatureOP> * templates);

        ~KMeanState();

        void gaussianTrain(int gaussianNum);
        double nodeCost(Feature *inputFeature);

		// lz:老的单高斯版本，用于测试
        void gaussianTrainTest(int gaussianNum);
        double nodeCostTest(Feature *inputFeature);

    private:
        // 对于Kmean， 存储的时候属于这个state的线段 
        // 注意是和基类的vector<WaveFeatureOP> 一一对应的
        std::vector< std::pair<int, int> > edgePoints;
		
		//lz:将该state内所有feature合并到points，
		//lz:并初始化gaussian参数
		void initTrain(int gaussianNum);
		//lz:
		Feature ** points;
		//lz: size of points
		int PointNum;
		int trainTime;
		int betterTime;
		//lz: 
		void KMeanTrain();


		double KMeanNodeCost(Feature * f);
        void generateInitFeature(Feature &initFeature);
		//lz
		//真正的模型
		std::vector<Gaussian*> GaussianModel;
		//临时的模型
		std::vector<Gaussian*> GaussianSet;
		double checkBetter(double);
		//临时的权重
		std::vector<double> weight;
		//真正的权重
		std::vector<double> w;	
		//lz: 释放高斯模型空间，清空weight和GaussianSet
		void clearGaussian();
		//lz: 计算第pid点最近的高斯模型编号
		int Point2Clusters(int pid);

        double u[39];
        double sigma[39];

};

#endif

