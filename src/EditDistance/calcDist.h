#ifndef __PRO3__EDITDISTANCE__CALC__
#define __PRO3__EDITDISTANCE__CALC__
#include <vector>
#include <cstring>
#include <string>
#include <map>
#include <set>
#include "conf.h"
#include <assert.h>
#include "tool.h"
#include <queue>
#include <iostream>
#include "path.h"
using namespace std;


int abs(int a){
	return a>0?a:-a;
}

int size(const char * t){
	return strlen(t);
}

template <typename T>
int size(const vector<T>& v){
	return v.size();
}

int min(int a,int b,int c){
	return a<b?(a<c?a:c):(b<c?b:c);
}


static void printMatrix(const char * a,const char * b,const Path* path){
	int sa = size(a);
	int sb = size(b);
	queue<int> q;
	
	q.push(0);
	//printf("0 - ");
	int now = 0;
	for(int i = path->len-1;i>=0;i--){
		if(path->dir[i] == DELETE){	now ++;}
		else if(path->dir[i] == INSERT){now += sa+1;}
		else{now += sa+2;}
		q.push(now);
	//	printf("%d - ",now);
	}
	//puts("");

	printf("    ");
	printf("    ");
	for(int i = 0;i<sa;i++){
		printf("   %c",a[i]);
	}	
	puts("\n");

	now = q.front();
	q.pop();
	int t = path->len-1;
	for(int i =0;i<=sb;i++){
		if(i-1>=0)printf("   %c",b[i-1]);
		else printf("    ");

		for(int j=0;j<=sa;j++){
			if(i*(sa+1)+j-1 == now && q.front() == i*(sa+1)+j){
				printf(LIGHT_RED " →" NONE);t--;
				printf("%2d",path->val[j*(sb+1)+i]);
				now = q.front();
				q.pop();
			}
			else{
				printf("%4d",path->val[j*(sb+1)+i]);
			}
		}

		printf("\n");
		printf("    ");
		for(int j =0;j<=sa;j++){
			if(i*(sa+1)+j == now && q.front() == (i+1)*(sa+1)+j){
				printf(LIGHT_GREEN "   ↓" NONE);t--;
				now = q.front();
				q.pop();
			}
			if(i*(sa+1)+j-1 == now && q.front() == (i+1)*(sa+1)+j){
				if(path->dir[t] == SAME)printf(" ↘  ");
				else printf(LIGHT_BLUE  " ↘  " NONE);
				t--;
				now = q.front();
				q.pop();
			}
			else{
				printf("    ");
			}	
		}
		printf("\n");
	}

}

template <typename T>
static void printPath(const T& a,const T& b,
				const Path* path,
				const char * split
)
{
	int da=0,db=0;
	for(int i = path->len-1;i>=0;i--){
		DIR d = path->dir[i];
		if(d==SAME){
			//printf(LIGHT_RED);
			if(strcmp(split,"\n")==0){printf("  ");}
			cout << a[da] << split;
			//printf(NONE);
			da++;db++;
		}
		else if(d==CHANGE){
			printf(LIGHT_BLUE);
			if(strcmp(split,"\n")==0){printf("* ");}
			if(strcmp(split,"\n")==0){
				cout << a[da] ;
				cout << " ---> " << b[db];
			}
			else {
				cout << b[db] ;	
			}
			cout << split;
			printf(NONE);
			da++;db++;
		}
		else if(d==INSERT){
			printf(LIGHT_GREEN);
			if(strcmp(split,"\n")==0){printf("+ ");}
			cout << b[db] << split;
			printf(NONE);
			db++;
		}
		else if(d==DELETE){
			printf(LIGHT_RED);
			if(strcmp(split,"\n")==0){printf("- ");}
			cout << a[da] << split;
			printf(NONE);
			da++;
		}
	}
	//puts("");	
}

void update(Path * path,int * val,DIR * p,int a_l,int b_l,int ret, int w){
		int px = a_l;
		int py = b_l;
		//printf("x : %d y : %d\n",px,py);

		int cnt =0;
		while(px || py){
			DIR d = p[px*(w+1)+py];
		//	printf("d - %d\n",d);
			path->add(d);
			if(d== INSERT) py--;
			else if(d== DELETE) px--;
			else {
				px--;py--;
			}
			if(d != SAME) cnt++;
		}
		if(ret!=cnt){
			printf("cost : %d path : %d\n",ret,cnt);
			path->print();	
		}
		assert(ret == cnt);
		//path->print();puts("");
		delete p;
		path->val = val;
	
}
// 普通比较算法
template <typename T>
static int compare(const T&  a,const T& b,Path* path = NULL){
	int a_l = size(a);
    int b_l = size(b);
 	   
	int * diff[2];
	DIR * p;
	int * val;
	if(path){
		if(a_l+b_l>5000){
			Warn("string too long, stop path!\n");
			return -1;
		}
		else{
			int ma = (a_l+1)*(b_l+1);
			p = new DIR[ma];
			val = new int[ma];
			memset(val,-1,sizeof(int)*(ma));
		}
	}

	diff[0] = new int[b_l+1];
	diff[1] = new int[b_l+1];

    for(int i = 0;i<=b_l;i++){	
		diff[0][i] = i;
		if(path){
			p[i] =  INSERT;
			val[i] = i;
		}
	}

    for(int i=1; i<=a_l;i++){
        diff[i&1][0] = i;

		if(path){
			p[i*(b_l+1)] =  DELETE;
			val[i*(b_l+1)] = i;
		}

        for(int j = 1;j<=b_l;j++){
			if(path){
				int in = diff[i&1][j-1]+1;
				int de = diff[(i&1)^1][j]+1;
				int d = diff[(i&1)^1][j-1];
				int bo = (a[i-1]!=b[j-1]);
				if(in<=de && in <=d+bo){
					diff[i&1][j] = in;
					p[i*(b_l+1)+j] =  INSERT;
					val[i*(b_l+1)+j] = in;
				}
				else if(de<=in && de<= d+bo){
					diff[i&1][j] = de;
					p[i*(b_l+1)+j] =  DELETE;
					val[i*(b_l+1)+j] = de;
				}
				else{
					diff[i&1][j] =d +bo;
					val[i*(b_l+1)+j] = d+bo;
					if(bo==1){
						p[i*(b_l+1)+j] =  CHANGE;
					}
					else{
						p[i*(b_l+1)+j] =  SAME;
					}
				}
			}
			else{
				diff[i&1][j] = min(
					diff[i&1][j-1]+1,
					diff[(i&1)^1][j]+1,
					diff[(i&1)^1][j-1] + (a[i-1]!=b[j-1])
				);
			}	
        }
	}
	
	int ret = diff[a_l&1][b_l];
	
	if(path){
		 update(path,val,p,a_l,b_l,ret,b_l);
	}

	
	delete diff[0];
	delete diff[1];
	return  ret;	
}

// 普通比较算法 无路径
/*template <typename T>
static int compare(const T&  a,const T& b){
	int a_l = size(a);
    int b_l = size(b);
    
	int * diff[2];
	diff[0] = new int[b_l+1];
	diff[1] = new int[b_l+1];

    for(int i = 0;i<=b_l;i++){diff[0][i] = i;}

    for(int i=1; i<=a_l;i++){
        diff[i&1][0] = i;
        for(int j = 1;j<=b_l;j++){
			diff[i&1][j] = min(
				diff[i&1][j-1]+1,
				diff[(i&1)^1][j]+1,
				diff[(i&1)^1][j-1] + (a[i-1]!=b[j-1])
			);
			//printf("%d %d %d\n",i,j,diff[i&1][j]);
        }
	}

	int ret = diff[a_l&1][b_l];
    delete diff[0];
	delete diff[1];
	return  ret;	
}
*/

static int refresh(int * pos,int * diff,int& cnt,int p,int val){
	if(cnt!=0 && p==pos[cnt-1]){
		if(diff[cnt-1]>val){
			diff[cnt-1]=val;
			return 1;
		}
		return 0;
	}
	else{
		pos[cnt] = p;
		diff[cnt] = val;
		cnt++;
		return 1;
	}
}

/*
template <typename T>
static int compareWithBeam(const T&  a,
							const T& b,
							int beam,
							int max_val
)
{
	int ret;
	int a_l = size(a);
    int b_l = size(b);
	int * diff[2];
	int * pos[2];
	int cnt[2] = {0};

	for(int i =0;i<2;i++){
		diff[i] = new int[b_l+1];
		pos[i] = new int[b_l+1];
	}
	
    for(int i = 0;i<=max_val&& i<=b_l;i++){
		diff[0][i] = i;
		pos[0][i] = i;
		cnt[0]++;
	}

	for(int i=1;i<=a_l;i++){
		if(i<=max_val){
			diff[i&1][0] = i;
			pos[i&1][0] = 0;
			cnt[i&1]=1;
		}
		else{
			cnt[i&1]=0;
		}

		for(int k=0;k<cnt[(i&1)^1];k++){
			int j=pos[(i&1)^1][k];
			int val = diff[(i&1)^1][k]+1;
			if(val<=max_val){
				refresh(pos[i&1],diff[i&1],cnt[i&1],j,val);
			}

			val = diff[(i&1)^1][k] + (a[i-1]!=b[j]);
			if(val<=max_val && j+1<=b_l){
				refresh(pos[i&1],diff[i&1],cnt[i&1],j+1,val);	
			}

			if(cnt[i&1]>0 && j+1 <=b_l){
				val = diff[i&1][cnt[i&1]-1]+1;
				if(val<=max_val)
					refresh(pos[i&1],diff[i&1],cnt[i&1],j+1,val);
			}
		}
		if(cnt[i&1]==0){
			for(int i=0;i<2;i++){
				delete diff[i];
				delete pos[i];
			}
			return  max_val+1;
		}
	}

	int c = cnt[a_l&1];
	if(c && pos[a_l&1][c-1] == b_l) {
		ret = diff[a_l&1][c-1];
	}
	else {
		ret = max_val+1;
    }

	for(int i=0;i<2;i++){
		delete diff[i];
		delete pos[i];
	}
	return  ret;	
}
*/
/*备份
template <typename T>
static int compareWithFixedThreshold(const T&  a,const T& b,int max_val){
	int ret;
	int a_l = size(a);
    int b_l = size(b);
	int * diff[2];
	int * pos[2];
	int cnt[2] = {0};

	for(int i =0;i<2;i++){
		diff[i] = new int[b_l+1];
		pos[i] = new int[b_l+1];
	}
	
    for(int i = 0;i<=max_val&& i<=b_l;i++){
		diff[0][i] = i;
		pos[0][i] = i;
		cnt[0]++;
	}

	for(int i=1;i<=a_l;i++){
		if(i<=max_val){
			diff[i&1][0] = i;
			pos[i&1][0] = 0;
			cnt[i&1]=1;
		}
		else{
			cnt[i&1]=0;
		}

		for(int k=0;k<cnt[(i&1)^1];k++){
			int j=pos[(i&1)^1][k];
			int val;

			//old更新右边,必须在new更新上面之前
			val = diff[(i&1)^1][k]+1; //获取old, k处更新值
			if(val<=max_val){
				refresh(pos[i&1],diff[i&1],cnt[i&1],j,val);
			}

			//new更新上面
			if(cnt[i&1]>0 && j+1 <=b_l){
				val = diff[i&1][cnt[i&1]-1]+1;
				if(val<=max_val)
					refresh(pos[i&1],diff[i&1],cnt[i&1],j+1,val);
			}

			//old更新对角线
			val = diff[(i&1)^1][k] + (a[i-1]!=b[j]);
			if(val<=max_val && j+1<=b_l){
				refresh(pos[i&1],diff[i&1],cnt[i&1],j+1,val);	
			}


		}
		if(cnt[i&1]==0){
			for(int i=0;i<2;i++){
				delete diff[i];
				delete pos[i];
			}
			return  max_val+1;
		}
	}

	int c = cnt[a_l&1];
	if(c && pos[a_l&1][c-1] == b_l) {
		ret = diff[a_l&1][c-1];
	}
	else {
		ret = max_val+1;
    }

	for(int i=0;i<2;i++){
		delete diff[i];
		delete pos[i];
	}
	return  ret;	
}
*/

template <typename T>
static int compareWithBeam(const T&  a,const T& b,
						int max_val,Path* path = NULL)
{
	int ret;
	int a_l = size(a);
    int b_l = size(b);
	int * diff[2];
	int * pos[2];
	int cnt[2] = {0};
	int aa = a_l,bb = b_l;
	DIR * p;
	int * val;
	if(path){
		if(a_l+b_l>5000){
			Warn("string too long, stop path!\n");
			return -1;
		}
		else{
			int ma = (a_l+1)*(b_l+1);
			p = new DIR[ma];
			val = new int[ma];
			memset(val,-1,sizeof(int)*(ma));
		}
	}

	for(int i =0;i<2;i++){
		diff[i] = new int[b_l+1];
		pos[i] = new int[b_l+1];
	}
	
    for(int i = 0;i<=max_val&& i<=b_l;i++){
		diff[0][i] = i;
		pos[0][i] = i;
		cnt[0]++;
		if(path){
			p[i]=INSERT;
			val[i] = i;
		}
	}

	int min_val = 0;
	for(int i=1;i<=a_l;i++){
		diff[i&1][0] = i;
		pos[i&1][0] = 0;
		cnt[i&1]=1;
		if(path){
			p[i*(b_l+1)] = DELETE;
			val[i*(b_l+1)] = i;
		}

		for(int k=0;k<cnt[(i&1)^1];k++){
			int j=pos[(i&1)^1][k];
			int r;
			int v;
			
			if(diff[(i&1)^1][k] > min_val+max_val)continue;
			//old更新右边,必须在new更新上面之前
			v = diff[(i&1)^1][k]+1; //获取old, k处更新值
			if(v<=min_val+max_val){
				r = refresh(pos[i&1],diff[i&1],cnt[i&1],j,v);
				if(path && r){
					p[i* (b_l+1) + j] = DELETE;
					val[i * (b_l+1) + j] = v;	
				}
			}

			//new更新上面
			if(cnt[i&1]>0 && j+1 <=b_l){
				v = diff[i&1][cnt[i&1]-1]+1;
				if(v<=max_val){
					r = refresh(pos[i&1],diff[i&1],cnt[i&1],j+1,v);
					if(path && r){
						p[i* (b_l+1) + j+1] = INSERT;
						val[i * (b_l+1) + j+1] = v;
					}
				}
			}

			//old更新对角线
			v = diff[(i&1)^1][k] + (a[i-1]!=b[j]);
			if(v<=max_val && j+1<=b_l){
				r = refresh(pos[i&1],diff[i&1],cnt[i&1],j+1,v);	
				if(path && r){
					p[i* (b_l+1) + j+1] =( (a[i-1]!=b[j])?CHANGE:SAME);
					val[i * (b_l+1) + j+1] = v;
				}
			}


		}

		if(cnt[i&1]==0){
			for(int t=0;t<2;t++){
				delete diff[t];
				delete pos[t];
			}
			ret = max_val+1;
			///!!!
			aa = i-1; bb = pos[i&1^1][cnt[i&1^1]-1];
			if(path){
				update(path,val,p,aa,bb,ret-1,b_l);
			}
			return  ret;
		}

		min_val = diff[i&1][0];
		for(int j = 1;j<cnt[i&1];j++){
			min_val = (min_val <= diff[i&1][j] ? min_val : diff[i&1][j]);
		}
	}
	
	int c = cnt[a_l&1];
	if(pos[a_l&1][c-1] == b_l) {
		ret = diff[a_l&1][c-1];
		if(path){
			 update(path,val,p,aa,bb,ret,b_l);
		}
	}
	else {
		aa = a_l-1;
		bb = pos[a_l&1][c-1];
		ret = max_val+1;

		if(path){
			 update(path,val,p,aa,bb,ret-1,b_l);
		}
    }


	for(int i=0;i<2;i++){
		delete diff[i];
		delete pos[i];
	}
	return  ret;	
}

template <typename T>
static int compareWithFixedThreshold(const T&  a,const T& b,
						int max_val,Path* path = NULL)
{
	int ret;
	int a_l = size(a);
    int b_l = size(b);
	int * diff[2];
	int * pos[2];
	int cnt[2] = {0};
	int aa = a_l,bb = b_l;
	DIR * p;
	int * val;
	if(path){
		if(a_l+b_l>5000){
			Warn("string too long, stop path!\n");
			return -1;
		}
		else{
			int ma = (a_l+1)*(b_l+1);
			p = new DIR[ma];
			val = new int[ma];
			memset(val,-1,sizeof(int)*(ma));
		}
	}

	for(int i =0;i<2;i++){
		diff[i] = new int[b_l+1];
		pos[i] = new int[b_l+1];
	}
	
    for(int i = 0;i<=max_val&& i<=b_l;i++){
		diff[0][i] = i;
		pos[0][i] = i;
		cnt[0]++;
		if(path){
			p[i]=INSERT;
			val[i] = i;
		}
	}

	for(int i=1;i<=a_l;i++){
		if(i<=max_val){
			diff[i&1][0] = i;
			pos[i&1][0] = 0;
			cnt[i&1]=1;
			if(path){
				p[i*(b_l+1)] = DELETE;
				val[i*(b_l+1)] = i;
			}
		}
		else{
			cnt[i&1]=0;
		}

		for(int k=0;k<cnt[(i&1)^1];k++){
			int j=pos[(i&1)^1][k];
			int r;
			int v;
			
			//old更新右边,必须在new更新上面之前
			v = diff[(i&1)^1][k]+1; //获取old, k处更新值
			if(v<=max_val){
				r = refresh(pos[i&1],diff[i&1],cnt[i&1],j,v);
				if(path && r){
					p[i* (b_l+1) + j] = DELETE;
					val[i * (b_l+1) + j] = v;
				}
			}

			//new更新上面
			if(cnt[i&1]>0 && j+1 <=b_l){
				v = diff[i&1][cnt[i&1]-1]+1;
				if(v<=max_val){
					r = refresh(pos[i&1],diff[i&1],cnt[i&1],j+1,v);
					if(path && r){
						p[i* (b_l+1) + j+1] = INSERT;
						val[i * (b_l+1) + j+1] = v;
					}
				}
			}

			//old更新对角线
			v = diff[(i&1)^1][k] + (a[i-1]!=b[j]);
			if(v<=max_val && j+1<=b_l){
				r = refresh(pos[i&1],diff[i&1],cnt[i&1],j+1,v);	
				if(path && r){
					p[i* (b_l+1) + j+1] =( (a[i-1]!=b[j])?CHANGE:SAME);
					val[i * (b_l+1) + j+1] = v;
				}
			}


		}
		if(cnt[i&1]==0){
			for(int t=0;t<2;t++){
				delete diff[t];
				delete pos[t];
			}
			ret = max_val+1;
			///!!!
			aa = i-1; bb = pos[i&1^1][cnt[i&1^1]-1];
			if(path){
				update(path,val,p,aa,bb,ret-1,b_l);
			}
			return  ret;
		}
	}
	
	int c = cnt[a_l&1];
	if(pos[a_l&1][c-1] == b_l) {
		ret = diff[a_l&1][c-1];
		if(path){
			 update(path,val,p,aa,bb,ret,b_l);
		}
	}
	else {
		aa = a_l-1;
		bb = pos[a_l&1][c-1];
		ret = max_val+1;

		if(path){
			 update(path,val,p,aa,bb,ret-1,b_l);
		}
    }


	for(int i=0;i<2;i++){
		delete diff[i];
		delete pos[i];
	}
	return  ret;	
}



//通过设固定阈值剪枝
template <typename T>
static int compare(const T&  a,const T& b,int max_val,Path * path = NULL){
	return compareWithFixedThreshold(a,b,max_val,path);
}

static int compareOneWithDict(const char * w,
							const vector<std::string>& dic,
							int type,
							Path * p
)
{
	int dic_l = dic.size();
    int now_best_val=(1<<30);
    int now_best_id=-1;
    
	for(int j=0;j<dic_l;j++){
        int tmp ;
		if(type == 1){
			if(abs(size(w)-(int)(dic[j].length()))<now_best_val){
				tmp = compare(w,dic[j].c_str());
			}
		}
		else if(type == 2){
			if(abs(size(w)-(int)(dic[j].length()))<now_best_val){
				tmp = compare(w,dic[j].c_str(),Conf::threshold);
				//tmp = compare(w,dic[j].c_str());
			}
			else tmp = Conf::threshold+1;
		}
		else if(type == 3){
			tmp = compareWithBeam(w,dic[j].c_str(),Conf::beam);
		}

        if(tmp<now_best_val){
            now_best_val = tmp;
            now_best_id = j;
        }
    }
	const string s = dic[now_best_id];
	int ma = s.length() + size(w) + 2;
	p->setMaxLen(ma);
	if(type==1)compare(w,s.c_str(),p);
    if(type==2)compare(w,s.c_str(),ma,p);
	if(type==3)compareWithBeam(w,s.c_str(),ma,p);
	return  now_best_id;
}
#endif
