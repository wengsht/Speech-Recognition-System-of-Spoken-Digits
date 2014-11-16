#include "spellcheck.h"
#include <string.h>
#include <string>

using namespace std;

SpellChecker::SpellChecker(){
	beam = INF;
}

SpellChecker::~SpellChecker(){
	
}

int SpellChecker::check(const char * word,bool one,bool use_path){
	this->one = one;
	if(one)use_path = false;

	if(use_path){
		this->path.init(&tree);
	}
	else{
		path.use = false;
	}

	Link * list = initLink();
	path.update();
	int now = 0;
	ansx = 0;
	int len = strlen(word);
	for(int i = 0;i<len;i++){
		refreshLink(word[i],list[now],list[now^1],one);
//		list[now^1].print();
		list[now].clear();
		now ^=1;
		path.update();
		ansx++;
	}
	
	list[now].checkSelf();
	
	int index,val;
	val = list[now].getMin(index);
	if(!one){
		path.create(ansx+1,0,ansx,index);
		path.update();
	}

	ansy = index;
	if(one)printf("%s ",getWord(index));
	delete [] list;
	return index;
}

// for check one word
Link * SpellChecker::initLink(){
	Link * list = new Link[2];
	list[0].init(&tree,beam);
	list[1].init(&tree,beam);
	//list[0].print();
	list[0].setFirstLink(this->path);
//	if(!one)path.update();
//	list[0].print();
	return list;
}

//for check one word
//update next link, and next link update itself (remove node with beam)
void SpellChecker::refreshLink(char next_c,
								Link& nowLink,
								Link& nextLink,
								bool one)
{
	int tmp;
	LinkNode * nowVal = ((nowLink.getVal()));
	LinkNode * nextVal = ((nextLink.getVal()));
//	printf("refresh\n");
	for(int nid = nowLink.getFirst();nid!=-1;nid=nowVal[nid].next){
		int node_val = nowVal[nid].val;
		int old_nid = nid;

		int max = nowLink.getMin(tmp)+beam;
		
		if(node_val>max)continue;
		// delete the letter
		
		bool f;
		//往右边更新
		if(old_nid>0 || one){
			f=nextLink.accept(old_nid,node_val+1,ansx);
			if(f)path.create(ansx+1,old_nid,ansx,old_nid);
		}
		else{
//			f=nextLink.accept(old_nid,INF,ansx);
			//if(f)path.create(ansx+1,old_nid,ansx,old_nid);
		}
		Node * node = tree[old_nid];

		int size = node->getSonSize();
		
		for(int j=0;j<size;j++){
			Node * son = node->getSon(j);
			int new_nid = son->getNid();

			if(next_c==son->getC()){// match the letter
				f=nextLink.accept(new_nid,node_val,ansx);		
			}
			else{// change the letter
				f=nextLink.accept(new_nid,node_val+1,ansx);		
			}
			if(f)path.create(ansx+1,new_nid,ansx,old_nid);
		}
		
	}

	//往子结点更新
	//map<int,int>& next_map = nextLink.getMap();
	for(int nid =nextLink.getFirst();nid!=-1;nid=nextVal[nid].next){
		if( (!one)&&(nid==0))continue;
		int tmp;
		int min_val = nextLink.getMin(tmp);
		int v = nextVal[nid].val+1;
		Node* p = tree[nid];
		int size = p->getSonSize();
		for(int i = 0;i<size;i++){
			Node * node = p->getSon(i);
			v = nextVal[nid].val+1;
			
			int son_id = node->getNid();
			bool f=nextLink.accept(son_id,v,ansx);
//			printf("work %d to %d with %d\n",son_id,nid,v);
			if(f)path.create(ansx+1,son_id,ansx+1,nid);	
		}
	}
	// update 最下方
	if(!one){
		for(int nid =nextLink.getFirst();nid!=-1;nid=nextVal[nid].next){
			Node* node = tree[nid];	
			if( (node->isLeaf())){
				int node_val = nextVal[nid].val;
				 bool f=nextLink.accept(0,node_val,ansx);
				 if(f)path.create(ansx+1,0,ansx+1,nid);
			}
		}
	}
}

