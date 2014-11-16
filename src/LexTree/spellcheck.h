#ifndef __PRO5__LEXTREE__SPELL_CHECK_H__
#define __PRO5__LEXTREE__SPELL_CHECK_H__
#include "LexTree.h"
#include <map>

const int INF = 0x3f3f3f3f;

typedef std::pair<int,int> pii;

struct Path{

	std::vector<pii> path;
	int * pre[2];
	int size;
	bool use ;
	int now ;
	LexTree * tree;
	
	int preNode;


	Path(){
		use = false;
		pre[0] = NULL;
		pre[1] = NULL;
	}

	~Path(){
		if(pre[0])delete [] pre[0];
		if(pre[1])delete [] pre[1];
	}

	void init(LexTree* tree){	
		this->tree = tree;
		this->size  = tree->getNodeSize();
		if(pre[0])delete [] pre[0];
		if(pre[1])delete [] pre[1];
		pre[0] = new int[size];
		pre[1] = new int[size];
		use = true;
		path.clear();
//		path.push_back(std::make_pair(-1,-1));
		now = 0;
		pre[0][0] = 0;
		preNode = -1;
	}

	void create(int x,int y,int px,int py){
		if(use == false)return;
	//	printf("%d %d -> %d %d\n",x,y,px,py);
		if(px == x){
			if(y==0){
				//pre[now][y] = path.size();
				preNode = py;
			}
			else{
				pre[now][y] = pre[now][py];	
			}
		}
		else{
			pre[now][y] = pre[now^1][py];
			if(py==0){
				pre[now][y] = path.size()-1;
				//preNode = py;
				//printf("px:%d py:%d\n",px,py);
			}
		}
	}

	void update(){
		if(use == false)return;
	//	printf("\nupdate:\n");
	//	printf("now:\n");
//		for(int i = 0;i<size;i++){
//			printf("%d\n",pre[now][i]);
//		}
///*
//		printf("now^1:\n");
//		for(int i = 0;i<size;i++){
//			printf("%d\n",pre[now^1][i]);
//		}
		//printf("preNode = %d\n",preNode);
//*/
		if(preNode == -1){
			path.push_back(std::make_pair(-1,-1));
		}
		else path.push_back(std::make_pair(pre[now][preNode],preNode));
		
	//	printf("-----------\n");
	//	for(int i = 0;i<path.size();i++){
	//		printf("%d  %d\n",i,path[i].first);
	//	}		
	//	printf("----------- end path\n");
		now^=1;
	}

	void print(int x,int y){
		if(use == false)return;
		std::vector<int> words;
		
		for(int i = path.size()-1;path[i].second!=-1;i=path[i].first){
	//		printf("%d ",i);
			words.push_back(path[i].second);
		}
		//printf("~~\n");
			
		for(int i = words.size()-1;i>=0;i--){
			printf("%s ",(*tree)[words[i]]->getWord());
		}printf("\n");
	}
};

//typedef map<int,int> Link;

struct LinkNode{
	int val;
	int next;
	LinkNode(){
		next = -1;
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
	int* lastUpdate;
	int firstNode;
	int lastNode;
	int size;

public:
	Link(){
		val = NULL;
		lastUpdate = NULL;
		clear();
	}
	~Link(){
		if(val)delete [] val;
		val = NULL;
		if(lastUpdate)delete [] lastUpdate;
		lastUpdate = NULL;
	}

	void clear(){
		//val.clear();
	//	if(val){
		//	for(int i = 0;i<size;i++){
		//		val[i].val = INF;
		//		val[i].next = -1;
		//	}
		//}

		firstNode = -1;
		lastNode = -1;
		now_min_val = INF;
		now_min_index = -1;
	}
	
	// 重新设置数组大小和min值,以及给入tree,beam
	void init(LexTree * tree,int beam){
		this->beam = beam;
		this->tree = tree;
		size = tree->getNodeSize();
		if(val)delete [] val;val=NULL;
		if(lastUpdate)delete [] lastUpdate;lastUpdate=NULL;
		
		val = new LinkNode [size];
		lastUpdate  = new int [size];	
		for(int i = 0;i<size;i++){
			lastUpdate[i] = -1;
			val[i].next=-1;
		}
		clear();
	}

	// 根据单词c与节点的编辑距离设置Link的值
	void setFirstLink(Path& p){
		val[0].val=0;
		firstNode = 0;
		lastNode = 0;

		now_min_val = 0;
		now_min_index = 0;

		for(int i = 1;i<size;i++){
			int pid = (*tree)[i]->getParent()->getNid();
			int v = val[pid].val+1;
			if(v>beam)continue;
			val[i].val=v;
			val[lastNode].next = i;
			lastNode = i;
	//		p.create(0,i,0,pid);
		}	
	}

	// 获得一个值，判断是否更新 !!
	// 并设置next
	bool accept(int nid,int v,int now_col){
		if(v>now_min_val+beam) return false;
		if(v<now_min_val){
			now_min_val = v;
			now_min_index  = nid;
		}

		if( lastUpdate[nid]<now_col){
			lastUpdate[nid] = now_col;
			val[nid].val = v;
			
			if(lastNode == -1){
				firstNode = nid;
			}
			else{
				val[lastNode].next=nid;
			}

			lastNode = nid;
			val[nid].next=-1;
			return true;
		}
		else{
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
		for (int nid = firstNode;nid!=-1;nid=val[nid].next){
			printf("%d %d\n",nid,val[nid].val);
		}

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
			if( (*tree)[nid]->isLeaf()){
				/*if(val[nid].val<now_min_val ||
				(val[nid].val == now_min_val && nid< now_min_index)){
					now_min_val = val[nid].val;
					now_min_index = nid;
				}*/
				Node * node = (*tree)[nid];
				Node *old_node = (*tree)[now_min_index];
				if(val[nid].val<now_min_val ||
				(val[nid].val == now_min_val && 
				strlen(old_node->getWord())< strlen(node->getWord()))){
					now_min_val = val[nid].val;
					now_min_index = nid;
				}
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
	void refreshLink(char next_c,Link& nowLink,Link& nextLink,bool one);
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
//		printf("x %d y %d\n",ansx,ansy);
		path.print(ansx,ansy);	
	}
};

#endif
