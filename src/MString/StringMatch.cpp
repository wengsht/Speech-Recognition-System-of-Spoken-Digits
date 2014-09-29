#include "StringMatch.h"
#include <string.h>
#include "resource.h"
#include "stringtool.h"
#include <queue>
#include <map>

using namespace std;

void StringMatch::print(vector<string> * v){
	for(int i = 0;i<v->size();i++){
		printf("%s ",(*v)[i].c_str());
	}
	puts("");
}

int StringMatch::initDict(const char * FileName){
	if(getStrsFromFile(&(this->dict),FileName,"",true)){
		int len = 0;
		int size = this->dict.size();
		for(int i = 0;i<size;i++){
			int tmp = dict[i].length();
			len = len > tmp ? len : tmp;
		}
		return len;
	}
	return -1;
}

int StringMatch::initWords(const char * FileName){
	if(getStrsFromFile(&(this->words),FileName,".,:!?\"",true)){
		int len = 0;
		int size = this->words.size();
		for(int i = 0;i<size;i++){
			int tmp = words[i].length();
			len = len > tmp ? len : tmp;
		}
		return len;
	}
	return -1;	
}

bool StringMatch::saveNewWordsToFileWithTemplate(
				const char * saveFileName,
				const char * templateFileName
				)
{
	if(saveStrsToFileByTemplate(
					&(this->new_words),
					saveFileName,
					templateFileName,
					".,:!?\"",
					true)== true) {
		return true;
	}
	return false;
}


int cnt;
///*
queue<pii> que;
int StringMatch::compare(const MCHAR * in,const MCHAR * tp){
	
	int len1 = strlen(in);
	int len2 = strlen(tp);
	
	initbuffer();

	que.push(mp(0,0));
	renew(0,0,0);
	
	while(!que.empty())
	{
		cnt--;

		pii t = que.front();
		que.pop();
		int newVal = get(t);
		if(newVal+1<=MAX_DIFF){
			if(t.second+1<=len2){
				pii tpos = mp(t.first,t.second+1);
				int re = renew(tpos,newVal+1);
				if(re == -1){
					que.push(tpos);
				}
			}
			if(t.first+1<=len1){
				pii tpos= mp(t.first+1,t.second);
				int re = renew(tpos,newVal+1);
				if(re == -1){
					que.push(tpos);
				}
			}
			if(t.first+1<=len1 && t.second+1<=len2){
				pii tpos= mp(t.first+1,t.second+1);
				int re  = renew(tpos,newVal+1);
				if(re == -1) {que.push(tpos);}	
			}
		}
		
		if(t.first+1<=len1 && t.second+1 <= len2 && in[t.first] == tp[t.second]){
			pii tpos(t.first+1,t.second+1);
			int re  = renew(tpos,newVal);
			if(re == -1) {que.push(tpos);}
		}	
		
	}
	if(cnt % 100000 == 0) printf("%d\n",cnt);
	return get(len1,len2);
}



int quick_compare(const MCHAR * in,const MCHAR * tp){
	int len1 = strlen(in);
	int len2 = strlen(tp);
	vector<int> diff[2];
	diff[0].resize(len2+1);
	diff[1].resize(len2+1);

//	vector<vector<Pos> >  pre;
	
	int now = 0;
	for(int i = 0;i<=len2;i++){
		diff[now][i] = i;
	}
	
	for(int i = 1; i<=len1;i++ , now ^= 1){
		diff[now^1][0] = i;
		for(int j = 1;j<=len2;j++){
			// delete one
			int tmp = -1;
			int d = diff[now^1][j-1];
			int l = diff[now][j];
			int ld = diff[now][j-1] ;


			//ok
			if(in[i-1] == tp[j-1] && ld>=0 && (tmp==-1 || tmp>ld)){tmp = ld;}	

			//delete one
			if((tmp==-1 || tmp > l+1) && l>=0) {tmp = l+1;}
			//change one
			if((tmp==-1 || tmp > ld+1) && ld>=0){tmp = ld+1;}
			//add one
			if((tmp==-1 || tmp > d+1) && d>=0) {tmp = d+1;}
	
			//Log("# diff[%d][%d] =  %d\n",i,j,diff[now^1][j]);
			diff[now^1][j] = tmp;

		}
	}

	int min = diff[now][len2];
	return min;
}
//*/

void StringMatch::run(const char * dictFile,const char * wordFile,const char * saveFile){
	dict_len = initDict(dictFile);

	words_len = initWords(wordFile);

	new_words.clear();
	
	SAFE_DELETE_POINTER(buffer);
	SAFE_DELETE_POINTER(path);
	
	buffer = new int[(words_len+1) * (dict_len+1)];
	
	path = new int[(words_len+1) * (dict_len+1)];

	Log("dict words size :%d",dict.size());
	Log("words size %d",words.size());
	
	int dict_len = 0;
	int words_len = 0;
	
	for(int i = 0;i<dict.size();i++){
		dict_len+=dict[i].length();
	}
	
	for(int i = 0;i<words.size();i++){
		words_len+=words[i].length();
	}
	
	cnt = dict_len * words_len;
	
	Log("Force Cal Times:%d",dict_len * words_len);
	
	for(int i = 0;i<words.size();i++){
		int min = -1;
		int min_index = -1;
		for(int j=0;j<dict.size();j++){
			int tmp = compare(words[i].c_str(),dict[j].c_str());
			int hold = quick_compare(words[i].c_str(),dict[j].c_str());
			if(tmp!=hold){
				for(int i = 0;i<5;i++){
					for(int j=0;j<5;j++){
						printf("%d ",get(i,j));
					}
					puts("");
				}

				ErrorLog("%s %s way1:%d way2:%d",
				words[i].c_str(),dict[j].c_str(),tmp,hold);
				return;
			}
			if(tmp == -1)continue;
			if(min == -1 || (tmp<min)){
				min  = tmp;
				min_index = j;
			}
		}
		
		if(min == -1){
			ErrorLog("can't find a string in dict for %s",words[i].c_str());
			return ;
		}

		new_words.push_back(dict[min_index]);
	}
	//printf("%d\n",sum);
	print(&new_words);
	Log("run over");

	saveNewWordsToFileWithTemplate(saveFile,wordFile);
}
