#include "Gaussian.h"
#include "mathtool.h"
#include <cmath>


Gaussian::Gaussian(int featureSize){
	this->featureSize = featureSize;
	this->mean = new double [featureSize];
	this->cvar = new double [featureSize];
	
	this->tmp_mean = new double [featureSize];
	this->tmp_mean2 = new double [featureSize];
	for(int i = 0;i<featureSize;i++){
		this->tmp_mean[i] = this->tmp_mean2[i] = 0;
	}
	sampleNum = 0;
	flag = true;
}

Gaussian::~Gaussian(){
	if(mean)delete mean;
	if(cvar)delete cvar;
}
int Gaussian::getflag(){return flag;}
void Gaussian::setMean(Feature * f){
	Feature & p = *f;
	for(int i = 0;i<featureSize;i++){
		mean[i] = p[i];
	}
}

void Gaussian::setCVar(double v){
	for(int i =0;i<featureSize;i++) cvar[i] = v;
}

void Gaussian::setRandCVar(){
	for(int i =0;i<featureSize;i++) cvar[i] = (rand()%1000/100000.0);
}

double Gaussian::getCVar(){
	double ret = 0;
	for(int i =0;i<featureSize;i++){
		ret+=cvar[i];
	}
	return ret;
}

void Gaussian::print(){
	printf("mean=[");
	for(int i = 0;i<featureSize;i++){
		printf("%lf ",mean[i]);
	}puts("]");
	printf("cvar=[");
	for(int i = 0;i<featureSize;i++){
		printf("%lf ",cvar[i]);
	}puts("]");
}
bool Gaussian::done(){
	double ep = 1e-12;
	bool converge = true;
	
	if(sampleNum == 0){
		return true;
	}

	for(int i = 0;i<featureSize;i++){
		tmp_mean[i] /= sampleNum;
		tmp_mean2[i]/= sampleNum;
		
		double v = tmp_mean2[i] - tmp_mean[i] * tmp_mean[i];
		double m = tmp_mean[i];
		if(fabs(v-cvar[i])>ep || fabs(m-mean[i])>ep){
			converge = false;
		}
		cvar[i] = v;
		mean[i] = m;
	}

	for(int i = 0;i<featureSize;i++){
		this->tmp_mean[i] = this->tmp_mean2[i] = 0;
	}
	sampleNum = 0;
	flag = true;
	return converge;
}

void Gaussian::addFeature(Feature * f, double probability) {
	Feature & p = *f;
	for(int i = 0;i<featureSize;i++){
		tmp_mean[i] += p[i] * probability;
		tmp_mean2[i] += p[i] *p[i] * probability;
	}
	sampleNum += probability;
}

double Gaussian::minuLogP(Feature* f){
	Feature & p = *f;
	double ret=0;
	double v,v2;
	double d;
	for(int i = 0;i<featureSize;i++){
		v = cvar[i];
		//v2 = v*v;
		d = p[i] - mean[i];
		ret+= log(2*PI*v)+(d*d/v);
	}
	ret = ret * 0.5;
	return ret;
}

// 与均值的欧式距离
double Gaussian::euDist(Feature* f){
    double ret = 0;
	for(int i = 0;i<featureSize;i++){
		ret += pow((*f)[i] - mean[i], 2.0);
	}
	return sqrt(ret);
}

void Gaussian::load(std::stringstream &in) {
    for(int i = 0;i < featureSize; i ++) {
        in >> mean[i];
        in >> cvar[i];
    }
    in >> flag;
}
void Gaussian::store(std::stringstream &out) {
    for(int i = 0;i < featureSize; i ++) {
        out << " " << mean[i];
        out << " " << cvar[i];
    }
    out << " " << flag;
}
