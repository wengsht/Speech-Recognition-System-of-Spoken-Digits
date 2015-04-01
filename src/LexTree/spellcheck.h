#ifndef __PRO5__LEXTREE__SPELL_CHECK_H__
#define __PRO5__LEXTREE__SPELL_CHECK_H__
#include "LexTree.h"
#include <map>

const int INF = 0x3f3f3f3f;

struct Path{
	std::map<int,int> path;
	int size;
	bool use ;
	Path(){
//		use = false;
	}
	void init(int size){
//		this->size = size;
//		use = true;
//		path.clear();
	}
	void create(int x,int y,int px,int py){
//		if(use==false)return;		
	}	
	void print(int x,int y){
//		if(use==false)return;
	}
};

//typedef map<int,int> Link;

struct LinkNode{
	int val;
	int next;
    int lastUpdate;
	LinkNode() {
		next = -1;
        lastUpdate = -1;
	}
};

class Link{
private:
	int beam;
	LexTree * tree;
	//// 
	int now_min_val;
	int now_min_index;

	//std::map<int,int> val;
	
	LinkNode * val;
	int firstNode;
	int size;

public:
	Link(){
		val = NULL;
		clear();
	}

	~Link(){
		if(val)delete [] val;
		val = NULL;
	}

	void clear(){
		//val.clear();
//		if(val) {
//			for(int i = 0;i<size;i++){
//				val[i].next = -1;
//			}
//		}

		firstNode = -1;
		now_min_val = INF;
		now_min_index = -1;
	}
	
	// 重新设置数组大小和min值,以及给入tree,beam
	void init(LexTree * tree,int beam){
		this->beam = beam;
		this->tree = tree;
		size = tree->getNodeSize();
		if(val) delete [] val;val=NULL;
		val = new LinkNode [size];
		clear();
	}

	// 根据单词c与节点的编辑距离设置Link的值
	void setFirstLink(){
		val[0].val=0;
		
        val[0].next = firstNode;
		firstNode = 0;

		now_min_val = 0;
		now_min_index = 0;

		for(int i = 1;i<size;i++){
			int pid = (*tree)[i]->getParent()->getNid();
			int v = val[pid].val+1;
			if(v>beam)continue;
			val[i].val=v;
			val[i].next = firstNode;
			firstNode = i;
		}					
	}

	// 获得一个值，判断是否更新 !!
	// 并设置next
	bool accept(int nid,int v, int column) {
		if(v>now_min_val+beam) return false;
		if(v<now_min_val){
			now_min_val = v;
			now_min_index  = nid;
		}

		if(val[nid].lastUpdate != column){
			val[nid].val = v;
            val[nid].lastUpdate = column;
			

            val[nid].next = firstNode;
            firstNode = nid;

			return true;
		}

		else {
			if(val[nid].val>v){
				val[nid].val=v;
				return true;
			}
		}
		return false;
	}

	void print(){
//		printf("\n|||||\n");
//		std::map<int,int>::iterator it;
//		for (it = val.begin();it!=val.end();it++){
//			printf("%d %s %d\n",it->first,(*tree)[it->first]->getWord(),it->second);
//		}
//		printf("min index = %d, min val = %d\n",now_min_index,now_min_val);
//		printf("|||||\n\n");

	}

	// 纵向更新
	void checkSelf(){
		now_min_val = INF;
		now_min_index = -1;
		for(int nid = firstNode;nid!=-1;nid=val[nid].next){
			//!!!
			if(val[nid].val<now_min_val && (*tree)[nid]->isLeaf()){
				now_min_val = val[nid].val;
				now_min_index = nid;
			}	
		}
	}

	///////////////////////////////////////////////////
	int getFirst(){
		return firstNode;
	} 
	LinkNode * getVal(){
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
	bool one;
	Path path;
	LexTree tree;
	int ansx,ansy;
protected:
	Link * initLink();
	void refreshLink(char next_c,Link& nowLink,Link& nextLink, int);
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
	int check(const char * word,bool one = false,bool use_path=true);
	void showInfo(){
		tree.showInfo();
	}
	void printAns(){
		
	}
    Link list[2];
};

#endif
