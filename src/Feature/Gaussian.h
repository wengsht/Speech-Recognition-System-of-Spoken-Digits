#ifndef _AUTOGUARD_Gaussian_H_
#define _AUTOGUARD_Gaussian_H_
#include <vector>
#include "Feature.h"
#include <sstream>
#include <cmath>

class Gaussian{
private:
	//均值
	double*  mean;
	//协方差，目前只考虑协方差的对角线以减少计算
	double*  cvar;
	//feature的维数，一般为39
	int featureSize;
	//
	//临时变量用于存储
	double* tmp_mean;
	double* tmp_mean2;
	double sampleNum;
	int flag;
public:
    static bool equal(double a, double b) {
        static double eps = 1e-4;

        return std::fabs(a-b) <= eps;
    }
    void load(std::stringstream &in);
    void store(std::stringstream &out);

	int getflag();
	Gaussian(int featureSize);
	~Gaussian();
	void print();	
	//添加一个数据点，即一个feature
	void addFeature(Feature * f, double probability = 1.0);
	//数据添加完毕重新计算mean和cvar
	//如果重新计算的mean和cvar小于阈值，则返回True(已经收敛)
	bool done();
	//计算在该模型下出现f的概率
	double minuLogP(Feature* f);

    double euDist(Feature *f);

	//用于初始化高斯的均值mean
	void setMean(Feature *f);
	//初始化协方差cvar，将cvar的对角线设为v
	void setCVar(double v);
	void copy(Gaussian * g, double dt){
		for(int i = 0;i<featureSize;i++){
			mean[i] = g->getm(i)+dt;
			cvar[i] = g->getv(i);
		}
	}

	double P(Feature*);
	bool update(std::vector<double>& u,std::vector<double> & v);

	double getm(int i){return mean[i];};
	double getv(int i){return cvar[i];};
	void setRandCVar();
	//获得协方差的总和
	double getCVar();
};

#endif
