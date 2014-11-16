#ifndef __PRO5__LEXTREE__SPELL_CHECK_H__
#define __PRO5__LEXTREE__SPELL_CHECK_H__
#include "LexTree.h"
#include <map>

const int INF = 0x3f3f3f3f;


struct PathNode{
	
};

struct Path{
	
};

//typedef map<int,int> Link;

class Link{
private:
	int beam;
	LexTree * tree;
	//// 
	int now_min_val;
	int now_min_index;

	std::map<int,int> val;

public:
	Link(){
		clear();
	}
	~Link(){
	}
	void clear(){
		val.clear();
		now_min_val = INF;
		now_min_index = -1;
	}
	
	// 重新设置数组大小和min值,以及给入tree,beam
	void init(LexTree * tree,int beam){
		this->beam = beam;
		this->tree = tree;
		clear();
	}

	// 根据单词c与节点的编辑距离设置Link的值
	void setFirstLink(){
		int size = tree->getNodeSize();
		val[0]=0;
		now_min_val = 0;
		now_min_index = 0;
		for(int i = 1;i<size;i++){
			int pid = (*tree)[i]->getParent()->getNid();
		//	printf("%d parent = %d\n",i,pid);
			std::map<int,int>::iterator it;
			if((it=val.find(pid))!=val.end()){
				int v = it->second+1;
				if (v>beam)continue;
				val[i] = v;
			}
		}					
	}

	// 获得一个值，判断是否更新 !!
	// 并设置next
	void accept(int nid,int v){
		if(v>now_min_val+beam) return;

		std::map<int,int>::iterator it;
		it = val.find(nid);
		if(it!=val.end()){
			if(it->second>v)it->second = v;	
		}
		else{	
			val[nid] = v;
		}

		if(v<now_min_val){
			now_min_val = v;
			now_min_index  = nid;
		}
	}

	void print(){
//		printf("\n|||||\n");
		std::map<int,int>::iterator it;
		for (it = val.begin();it!=val.end();it++){
			printf("%d %s %d\n",it->first,(*tree)[it->first]->getWord(),it->second);
		}
		printf("min index = %d, min val = %d\n",now_min_index,now_min_val);
//		printf("|||||\n\n");

	}

	// 纵向更新，同时做beam
	void checkSelf(bool last = false){
		std::map<int,int>::iterator it;
		for(it = val.begin();it!=val.end();it++){
			if(it->second<now_min_val){
				now_min_val=it->second;
				now_min_index=it->first;
			}
			
			if(it->second+1>beam)continue;
			Node * p = (*tree)[it->first];
			int size = p->getSonSize();
			for(int i = 0;i<size;i++){
				// insert the letter
				accept(p->getSon(i)->getNid(),it->second+1);
			}
		}
	
			
		
		it = val.begin();
		std::map<int,int>::iterator end = val.end();
		std::map<int,int>::iterator tmp;
		int max = now_min_val+beam;
		//printf("max : %d last:%d\n",max,last);
		if(last){
			now_min_val = INF;
			now_min_index = -1;
		}
		while(it!=end){
			if(last){
				if(it->second<now_min_val && (*tree)[it->first]->isLeaf()){
					now_min_val = it->second;
					now_min_index = it->first;
				}
				it++;
			}
			else if(it->second>max){
				tmp = it;
				it++;
				val.erase(tmp);
		//		printf("do erase");
				end = val.end();
			}
			else it++;
		}
	}

	///////////////////////////////////////////////////
	std::map<int,int>& getMap(){
		return val;
	} 
	int getMin(int& index){
		index = now_min_index;
		return now_min_val;
	}
};


class SpellChecker
{
private:
	LexTree tree;
protected:
	Link * initLink();
	void refreshLink(char next_c,Link& nowLink,Link& nextLink,bool last=false);
	int beam;
public:
	SpellChecker(const char * dicFileName,int beam){	
		tree.buildTree(dicFileName);
		this->beam = beam;
	}
	char * getWord(int index){
		return tree[index]->getWord();
	}	
	void setBeam(int b){beam = b;}
	SpellChecker();
	~SpellChecker();
	int checkOneWord(const char * word);
	void showInfo(){
		tree.showInfo();
	}	
};

#endif
