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

struct Cluster{
	Gaussian * g;
	std::vector<Feature*> points;
	double w;
};


class KMeanState : public HMMState {
    public:
        const static std::pair<int, int> NullSeg ;
        friend class HMMKMeanAutomaton;
        friend class HMMAutomaton;
        friend class SeqModel;

        KMeanState(std::vector<WaveFeatureOP> * templates);

        ~KMeanState();

        void gaussianTrain(int gaussianNum);
        double nodeCost(Feature *inputFeature);

        void load(std::stringstream &in, int &gaussNum);
        void store(std::stringstream &out);
        void setTemplates(std::vector<WaveFeatureOP> * newTemps);
    private:
        // 对于Kmean， 存储的时候属于这个state的线段 
        // 注意是和基类的vector<WaveFeatureOP> 一一对应的
        std::vector< std::pair<int, int> > edgePoints;
		
		//所有的Cluster，一个Cluster一个高斯模型，仅用于split过程
		//完成后抛弃所有points，将w和g分别存入到真正的权重和模型
		std::vector<Cluster*> ClusterSet;
		//真正的模型
		std::vector<Gaussian*> GaussianModel;
		//真正的权重
		std::vector<double> w;	
		
		std::vector<Feature*> points; 


	private: //方法
		void printw();
		void clearGaussian();
		void clearCluster();
		// 一次训练
		bool KMeanTrain(int &gaussianNum);

		double sumCV();
		
		Cluster* findBigCluster();	
		void deleteCluster(Cluster*c);

		//split 函数
		bool addTwoCluster(const Cluster* c, double smallNum);

		
		Gaussian * initRandGaussian(const std::vector<Feature*>& p)const;
		
		void saveGaussianModel();

		int calcFirstCluster();

		void generateInitFeature(Feature &initFeature,
						const std::vector<Feature*>& points)const;
		double KMeanNodeCost(Feature * f);
		
		void EM(int g);
};



#endif

