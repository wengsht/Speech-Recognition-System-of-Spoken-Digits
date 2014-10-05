#ifndef __PRO3__EDITDISTANCE__CALC__
#define __PRO3__EDITDISTANCE__CALC__
#include <vector>
#include <cstring>
#include <string>
#include "conf.h"

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


template <typename T>
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


static void refresh(int * pos,int * diff,int& cnt,int p,int val){
	if(cnt!=0 && p==pos[cnt-1]){
		diff[cnt-1]>val?diff[cnt-1]=val:0;
	}
	else{
		pos[cnt] = p;
		diff[cnt] = val;
		cnt++;
	}
}


template <typename T>
static int compareWithOptThreshold(const T&  a,const T& b,int max_val){
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
//		printf("%d : ",i);
//		for(int t=0;t<cnt[i&1];t++){
//			printf("%d-%d ",pos[i&1][t],diff[i&1][t]);
//		}puts("");
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


template <typename T>
static int compare(const T&  a,const T& b,int max_val){
	return compareWithOptThreshold(a,b,max_val);
}

static int compareOneWithDict(const char * w,
							const vector<std::string>& dic,
							int type
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

        if(tmp<now_best_val){
            now_best_val = tmp;
            now_best_id = j;
        }
    }
    return  now_best_id;
}
#endif
