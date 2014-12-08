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
#include <cstdlib>
#include "mathtool.h"
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <vector>
#include "configure_feature.h"
using namespace std;

const std::pair<int, int> KMeanState::NullSeg = std::make_pair(0, -1);

KMeanState::KMeanState(std::vector<WaveFeatureOP> * templates) : HMMState(templates) {
    if(templates)
        edgePoints.resize(templates->size());

    srand(time(0));
	ClusterSet.clear();
}
void KMeanState::setTemplates(std::vector<WaveFeatureOP> * newTemps) {
    templates = newTemps;

    if(templates)
        edgePoints.resize(templates->size());
}


KMeanState::~KMeanState() 
{
	clearGaussian();
	clearCluster();
}

//清空高斯模型（包括w）
void KMeanState::clearGaussian()
{
	for(int i = 0;i<GaussianModel.size();i++){
		if(GaussianModel[i])delete GaussianModel[i];
	}	

	GaussianModel.clear();
	w.clear();
}

//释放空间，
//但不一定释放高斯g，
//因为可能已经转移到GaussianModel
//此时一定记得将g设为NULL
void KMeanState::clearCluster(){
	int s = ClusterSet.size();
	for(int i = 0;i<s;i++){
		if(ClusterSet[i]->g) delete ClusterSet[i]->g;
		delete ClusterSet[i];
	}
	ClusterSet.clear();
}
void KMeanState::printw(){	
	for(int i = 0;i<GaussianModel.size();i++){	
		printf(" %lf ",w[i]);	
	}
	printf("\n");
}
// gaussianNum 为期望高斯数目，如果点不够，将会变少。
void KMeanState::gaussianTrain(int gaussianNum) {
//    printf("2 %d\n", gaussianNum);
	int time = 1;
	double best = -1;
	
	clearGaussian();
	clearCluster();

	while(time--) {
		//printf("Gaussian Train time %d\n",time);
		if(KMeanTrain(gaussianNum)==false) return;
//        printf("%d\n", points.size());
//        if(gaussianNum >= 4)
//printf("%lf\n", ClusterSet[3]->w);
		double tmp  = sumCV();

		if(best == -1 || tmp < best){
			best = tmp;
			//使用完后本轮ClusterSet作废
			saveGaussianModel();
//			printf("old:");printw();
			//前面部分其实只是初始化？
//			printf("%lf-->",best);
		}
		clearCluster();
	}

//    printf("%lf\n", w[1]);
//    printf("%d\n", points.size());
//    printf("%d\n", gaussianNum);
//	EM(gaussianNum);
//	printf("new:");printw();
//	printf("\n");
}

//一轮训练
//0个节点返回false
//必然训练出大于等于1个的高斯模型，返回true
bool KMeanState::KMeanTrain(int &gaussianNum) {
	int pointNum = calcFirstCluster();
//    printf("%d\n", pointNum);
//	printf("Point Num : %d\n",pointNum);

	if(pointNum == 0) return false;

	if(pointNum < gaussianNum) {
		gaussianNum = 1; //pointNum;
	}

	//只有一个高斯模型时
	ClusterSet[0]->w = 1.0;
	Gaussian * g = new Gaussian(ClusterSet[0]->points[0]->size());
		
	vector<Feature*> &f = ClusterSet[0]->points;
		
	for(int i = 0;i<f.size();i++){
		g->addFeature(f[i]);
	}

	g->done();	
		
	ClusterSet[0]->g = g;
	
	if(gaussianNum == 1) 
        return true;
	
	for(int i = 1;i<gaussianNum;i++) {
		Cluster* c = findBigCluster();
		double w = c->w;
		
        double smallNum = 1;
		while(addTwoCluster(c, smallNum)==false) {
//            printf("%lf\n", smallNum);
            smallNum /= 2.0;
        }
	
		deleteCluster(c);
	}
	
//    printf("%lf\n", ClusterSet[1]->w);
	return true;	
}

//更新gussianModel 和w
void KMeanState::EM(int g) {	
//    printf("%lf\n", w[1]);
    if(g <= 1) return ;
	bool converge = false;
	
	int N = points.size();

    if(N == 0) return ;

	int fsize = points[0]->size();
//	printf("EM %d %d %d\n",g,N,fsize);
    Matrix<double> u, v, p;
    Matrix<double> tmpCost;
    u.resize(g); v.resize(g); p.resize(N);
    tmpCost.resize(N);

    /*
	vector<double> *u = new vector<double>[g];
	vector<double> *v = new vector<double>[g];
	vector<double> *p = new vector<double>[N]; //p[i][j]第i点属于j模型的概率
    */
	double *pg = new double[g]; //属于第g个模型的p总和。

	for(int i = 0;i<N;i++) {
        p[i].resize(g);
        tmpCost[i].resize(g);
    }
	for(int i = 0;i<g;i++) {
		u[i].resize(fsize);
		v[i].resize(fsize);
	}

	int cnt = 300;

	while( (!converge) && cnt-- ) {
		//初始化
		converge = true;
		for(int i = 0;i<g;i++)
            pg[i] = 0;
		
		//计算pij,pg
//		puts("calc pij");
		for(int i = 0;i<N;i++){
			double sumCost = Feature::IllegalDist;

			for(int j=0;j<g;j++){
                tmpCost[i][j] = p2cost(w[j]) + GaussianModel[j]->minuLogP(points[i]);
//				p[i][j] = w[j] * GaussianModel[j]->P(points[i]);

                sumCost = logInsideSum(sumCost, tmpCost[i][j]);
//				sumi += p[i][j]; 
			}

			for(int j=0;j<g;j++) {
                tmpCost[i][j] -= sumCost;
                p[i][j] = cost2p(tmpCost[i][j]);
//				p[i][j]/=sumi;
				pg[j] += p[i][j];
			}
		}

		double tmp;

		for(int j =0;j<g;j++){
//			printf("calc G %d",g);			
			//计算wi
			tmp = 1.0/N * pg[j];
			//printf("w = %.12lf ",fabs(w[j]-tmp));
			if(fabs(w[j]-tmp)>1e-10) converge = false;

			w[j] = tmp;

			for(int k = 0;k<fsize;k++){
				u[j][k] = 0;
				v[j][k] = 0;
			}
	
//			puts("calc u");
			//计算u new
			for(int k=0;k<fsize;k++){
				for(int i = 0;i<N;i++){
					u[j][k]+=(*points[i])[k] * p[i][j];
				}
				u[j][k] /= pg[j];
			}

//			puts("calc v");
			//计算v new
			for(int k = 0;k<fsize;k++) {
				for(int i = 0;i<N;i++) {
					tmp = ((*points[i])[k] - u[j][k]);
					v[j][k] += tmp*tmp*p[i][j];
				}
				v[j][k] /= pg[j];
			}
			
			converge &= GaussianModel[j]->update(u[j],v[j]);
		}
	}
	//printf("cnt = %d\n",cnt);
    /*  

	delete [] u;
	delete [] v;
	delete [] p;
    */
	delete [] pg;
}

//找最大权值的Cluster 用于Split
//确保Cluster数目大于0
Cluster* KMeanState::findBigCluster()
{
	int s = ClusterSet.size();
	double maxw = ClusterSet[0]->w; 
	int index = 0;
	for(int i = 1;i<s;i++){
		if(maxw < ClusterSet[i]->w){
			index = i;
			maxw = ClusterSet[i]->w;
		}
	}
	return ClusterSet[index];
}

//不包括w，仅仅是方差和均值
//保证p个数大于0
Gaussian * KMeanState::initRandGaussian(const vector<Feature*>& p)const
{
	Gaussian * g = new Gaussian(p[0]->size());	
    //Feature initFeature;

	//generateInitFeature(initFeature,p);
	g->setMean(p[rand()%(p.size())]);
	//g->setMean(&initFeature);
	g->setRandCVar();

	return g;
}

//将点集合p分成两个新的Cluster,用kmean找出2个Gaussian分布g1,g2和w1,w2
//w1 = wt*w1 w2 = wt*w2
//并存放在ClusterSet里
//点集合也被分开
//返回false: 收敛到一个高斯而不是2个，需要重做
bool KMeanState::addTwoCluster(const Cluster * c, double smallNum)
{
    const static double eps = 1e-5;

	const vector<Feature*>& p = c->points;
	const double wt = c->w;

	//Gaussian * g1 = initRandGaussian(p);
	//Gaussian * g2 = initRandGaussian(p);
	
	Gaussian * g1 = new Gaussian(p[0]->size());
	Gaussian * g2 = new Gaussian(p[0]->size());

	g1->copy(c->g, smallNum); //rand()%100/200.0);
	g2->copy(c->g, -smallNum); //-rand()%100/200.0);

//    printf("%lf\n", smallNum);
	int w1 = 0;
	int w2 = 0;
	Cluster* c1 = new Cluster();
	Cluster* c2 = new Cluster();
	//printf("%d = ",p.size());	
	while(true) {
		//每点分配到一个模型
		for(int i = 0;i<p.size();i++){
			double d1 = g1->minuLogP(p[i]);
			double d2 = g2->minuLogP(p[i]);

			if((smallNum < eps && (i&1)) || (smallNum >= eps && d1<d2)) {
				c1->points.push_back(p[i]);
				w1++;
				g1->addFeature(p[i]);
			}
			else {
				c2->points.push_back(p[i]);
				w2++;
				g2->addFeature(p[i]);
			}
		}
		//重新计算mean 和 cvar，并判断是否收敛
		if(g1->done() && g2->done()) break;
        break;
//        if(smallNum < eps) break;
	//	g1->print();
	//	g2->print();
		//为收敛准备重做
		c1->points.clear();c2->points.clear();
		w1 = w2 = 0;
	}

//	printf("%d+%d ---%lf\n",w1,w2,wt);
	if(w1==0 || w2==0){
		if(c1)delete c1;
		if(c2)delete c2;
		if(g1)delete g1;
		if(g2)delete g2;
//	printf("%d+%d ---%lf\n",w1,w2,wt);

		return false;
	}

	double w = (double)(p.size());

	c1->w = w1/w*wt; c1->g=g1;
	c2->w = w2/w*wt; c2->g=g2;

//    printf("%lf\n", c1->w);
	ClusterSet.push_back(c1);
	ClusterSet.push_back(c2);
	return true;
}


//从vector中删除，释放高斯，释放c空间
void KMeanState::deleteCluster(Cluster * c)
{
	if(c==NULL)return;
	if(c->g)delete (c->g);
	vector<Cluster *>::iterator pos;
    for(pos = ClusterSet.begin(); pos != ClusterSet.end(); pos++) {
        if(*pos == c) {
            ClusterSet.erase( pos );
            break;
        }
    }
    /*  
	pos = find(ClusterSet.begin(),ClusterSet.end(),c);
	if(pos != ClusterSet.end()){
		ClusterSet.erase(pos);
	}
    */

	delete c;
}


//所有cluster中g的加权w方差和
double KMeanState::sumCV()
{
	//double w = -1;
	double w = 0;
	int s= ClusterSet.size();
	
	for(int i =0;i<s;i++){
		w+= (ClusterSet[i]->w) * (ClusterSet[i]->g->getCVar());				
		//double t = (ClusterSet[i]->w) * (ClusterSet[i]->g->getCVar());			
		//if(w<0 || w> t){w=t;}
	}

	return w;
}

//将ClusterSet中的混合模型保存到真正的模型和!!权重
//将Cluster设置为NULL!!
void KMeanState::saveGaussianModel()
{
	clearGaussian();
	int s = ClusterSet.size();
	for(int i = 0;i<s;i++){
		GaussianModel.push_back(ClusterSet[i]->g);
		w.push_back(ClusterSet[i]->w);
//        printf("%lf\n", ClusterSet[i]->w);
		ClusterSet[i]->g = NULL;
	}
}

//new出第一个Cluster 并将p,w=1,g=NULL设置好
//ok
int KMeanState::calcFirstCluster(){
	Cluster* c = NULL;
	points.clear();
//    printf("%d\n", (c->points).size());

	for(int i = 0;i<edgePoints.size();i++){
		int st = edgePoints[i].first;
		int ed = edgePoints[i].second;

        if(edgePoints[i] == NullSeg) continue;
//        printf("%d %d\n", st, ed);
//        if(st == 0 && ed == -1)

//        printf("%p %d xx %d\n",this, i, points.size());
//        printf("%d %d\n", st, ed);

		for(int j = st;j<=ed;j++){
			if(c==NULL){	
				c = new Cluster();
				c->w = 1;
				c->g = NULL;
			}
			Feature * f = &((*templates)[i][j]);
			(c->points).push_back(f);
			points.push_back(f);
		}
	}

    /*  
    if(this->edgePoints[0] == NullSeg && 
            this->edgePoints[1] == NullSeg && 
            this->edgePoints[2] == NullSeg) {
       printf(" .. %d\n", points.size());
    }
    */

	if(c!=NULL){
		ClusterSet.push_back(c);
		return (c->points).size();
	}

	return 0;
}

///////////////////////////////////////////////////////////

void KMeanState::generateInitFeature(Feature &initFeature,
					const vector<Feature*>& points)const 
{
	int PointNum = points.size();
	if(PointNum==0)return;

    int featureSiz = points[0]->size();

    initFeature.resize(featureSiz);

    double *minD = new double[featureSiz];
    double *maxD = new double[featureSiz];

    for(int j = 0;j < featureSiz; j++) {
        minD[j] = maxD[j] = (*(points[0]))[j];
    }

    for(int j = 0;j < initFeature.size(); j++) {
        for(int i = 1;i < PointNum; i++) {
            minD[j] = fmin(minD[j], (*(points[i]))[j]);
            maxD[j] = fmax(maxD[j], (*(points[i]))[j]);
        }
    }

    for(int j = 0;j < initFeature.size(); j++) {
        initFeature[j] = 
			minD[j] + (maxD[j] - minD[j]) * 
			(1.0 *(rand() % 10000) / 10000);
    }

    delete []minD;
    delete []maxD;
}
////////////////////////////////////////////////////////////////over


//////////////////////////////////////////////////////////////////

//返回该模型的NodeCost
double KMeanState::nodeCost(Feature *inputFeature) {
	return this->KMeanNodeCost(inputFeature);
}

double KMeanState::KMeanNodeCost(Feature *f){
	int gn = GaussianModel.size();

	if( gn == 0 ) return Feature::IllegalDist;

	double ret = Feature::IllegalDist;
	const double eps = 1e-13;
	
	for(int i = 0;i<GaussianModel.size();i++) {
		if(fabs(w[i]) < eps) continue;

		double t = GaussianModel[i]->minuLogP(f) - log(w[i]);
		if(ret == Feature::IllegalDist) {
            ret = t;
        }
		else {
            ret = logInsideSum(ret,t);
        }
	}
	return ret;
}

void KMeanState::load(std::stringstream &in, int &gaussNum) {
    clearGaussian();
//    if(templates->size() <= 0)
//        return ;

//    int featureSize = (*templates)[0][0].size();
    // some time it will not load mfcc into memory(because hmm model is read from file!!)

    int featureSize = CEPS_NUM * (DELTA_TIMES + 1);

    in >> gaussNum;
    w.resize(gaussNum);

    for(int i = 0;i < gaussNum; i++) {
        Gaussian * g = new  Gaussian(featureSize);
        in >> w[i];

        g->load(in);
        GaussianModel.push_back(g);
    }
//    puts("");
}

void KMeanState::store(std::stringstream &out) {
    out << " " << GaussianModel.size();

    for(int i = 0;i < GaussianModel.size(); i++) {
        out << " " << w[i];

        GaussianModel[i]->store(out);
    }
//    puts("");
}
