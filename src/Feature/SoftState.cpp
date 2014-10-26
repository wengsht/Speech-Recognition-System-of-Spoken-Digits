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
#include <ctime>
#include "mathtool.h"
#include <cstdlib>

SoftState::SoftState(std::vector<WaveFeatureOP> * templates) : HMMState(templates) {
    probabilities.resize(templates->size());

    srand(time(0));
    int idx;
    for(idx = 0; idx < templates->size(); idx ++) {
        probabilities[idx].resize((*templates)[idx].size());
    }
}

SoftState::~SoftState() {
    clearGaussian();
}

void SoftState::gaussianTrainTest(int gaussianNum) {
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

double SoftState::nodeCostTest(Feature *inputFeature) {
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

void SoftState::clearGaussian() {
    for(int i = 0;i < GaussianSet.size(); i++) 
        delete GaussianSet[i];

    GaussianSet.clear();

    weight.clear();
}
void SoftState::initTrain(int gaussianNum) {
    totalProbability = 0.0;
    int pointCnt = 0;

    for(int i = 0;i < probabilities.size(); i++) {
        for(int j = 0; j <  probabilities[i].size(); j++) {
            totalProbability += probabilities[i].size();
            if(probabilities[i][j] > 0.0)
                pointCnt ++;
        }
    }

    clearGaussian();

    if(pointCnt < gaussianNum) 
        gaussianNum = 1;
    if(pointCnt == 0) {
        gaussianNum = 0;
        return ;
    }

    int featureSize = (*templates)[0][0].size();

    for(int i = 0;i < gaussianNum; i++) {
		Gaussian * g = new  Gaussian(featureSize);
        // TODO init?
        int temIdx = rand() % (*templates).size();
        int feaIdx = rand() % (*templates)[temIdx].size();
		g->setMean(&((*templates)[temIdx][feaIdx]));
		g->setCVar(3);
		weight.push_back(1.0/gaussianNum);
		GaussianSet.push_back(g);
    }
}

void SoftState::gaussianTrain(int gaussianNum) {
    initTrain(gaussianNum);

    this->SoftTrain();
}

double SoftState::nodeCost(Feature *inputFeature) {
    return this->SoftNodeCost(inputFeature);
}

int SoftState::Point2Clusters(int templateIdx, int featureIdx) {
	double mindist = GaussianSet[0]->minuLogP(&((*templates)[templateIdx][featureIdx]));
	int gid = 0;
	for(int i = 1;i<GaussianSet.size();i++){
		double mlp = GaussianSet[i]->minuLogP(&((*templates)[templateIdx][featureIdx]));

		if(mlp < mindist ){
			mindist = mlp;
			gid  = i;
		}
	}
	return gid;
}

void SoftState::SoftTrain() {
	if(GaussianSet.size()==0)return;
	bool converge = false;
	
	while(!converge){
        std::vector<double> count;
	
		for(int i = 0;i<GaussianSet.size();i++) {
			count.push_back(0.0);
		}

		converge = true;
		// 计算每一点属于哪个gaussian，并统计数据
        for(int templateIdx = 0; templateIdx < templates->size(); templateIdx++) {
            for(int featureIdx = 0; featureIdx < (*templates)[templateIdx].size(); featureIdx ++) {
			    int gid = Point2Clusters(templateIdx, featureIdx);
			    GaussianSet[gid]->addFeature(&((*templates)[templateIdx][featureIdx]));
			    count[gid] += probabilities[templateIdx][featureIdx];
            }
        }

		// 更新参数并计算是否已经收敛
		for(int i = 0;i < GaussianSet.size();i++){
			double wi = count[i] / totalProbability;

			converge &= GaussianSet[i]->done();
			if(fabs(wi-weight[i]) > 0.1){
				converge = false;
			}
			weight[i] = wi;
		}
	}
}

double SoftState::SoftNodeCost(Feature *f) {
	if(GaussianSet.size()==0)return Feature::IllegalDist;
	
	double ret = Feature::IllegalDist;
	const double eps = 1e-13;
	for(int i = 0;i < GaussianSet.size();i++){
		if(fabs(weight[i])<eps) continue;
		double t = GaussianSet[i]->minuLogP(f);
		t -= log(weight[i]);

		ret = logInsideSum(ret,t);
	}
	return ret;
}
