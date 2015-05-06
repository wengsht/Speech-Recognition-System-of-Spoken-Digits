#include "spellcheck.h"
#include <string.h>
#include <string>

using namespace std;

SpellChecker::SpellChecker(){
	beam = INF;
}

SpellChecker::~SpellChecker(){
//    delete [] list;

}

int SpellChecker::check(const char * word,bool one,bool use_path){
	this->one = one;

//	if(use_path){
//		this->path.init(tree.getNodeSize());
//	}
//	else{
//		path.use = false;
//	}

	initLink();

	int now = 0;
	ansx = 0;
	int len = strlen(word);
	for(int i = 0;i<len;i++){
		refreshLink(word[i],list[now],list[now^1], i);
		
		list[now].clear();
		now ^=1;
		ansx++;
	}

	list[now].checkSelf();
	
	int index,val;
	val = list[now].getMin(index);
	/// !!!
	ansy = index;
	printf("%s ",getWord(index));
//	delete [] list;
	return index;
}

// for check one word
Link * SpellChecker::initLink(){
//    if(list) return list;

//	Link * list2 = new Link[2];
	list[0].init(&tree,beam);
	list[1].init(&tree,beam);
	//list[0].print();
	list[0].setFirstLink();
//	list[0].print();
	return list;
}

//for check one word
//update next link, and next link update itself (remove node with beam)
void SpellChecker::refreshLink(char next_c,
								Link& nowLink,
								Link& nextLink
                                , int column
								)
{
	int tmp;
	LinkNode * nowVal = ((nowLink.getVal()));
	LinkNode * nextVal = ((nextLink.getVal()));

	for(int nid = nowLink.getFirst();nid!=-1;nid=nowVal[nid].next){
		int node_val = nowVal[nid].val;
		int old_nid = nid;

		int max = nowLink.getMin(tmp)+beam;
		
		if(node_val>max)continue;
		// delete the letter
		
		bool f=nextLink.accept(old_nid,node_val+1, column);
		if(f)path.create(ansx+1,old_nid,ansx,old_nid);

		Node * node = tree[old_nid];

		int size = node->getSonSize();
		
		for(int j=0;j<size;j++){
			Node * son = node->getSon(j);
			int new_nid = son->getNid();

			if(next_c==son->getC()){// match the letter
				f=nextLink.accept(new_nid,node_val, column);		
			}
			else{// change the letter
				f=nextLink.accept(new_nid,node_val+1, column);		
			}

			if(f)path.create(ansx+1,new_nid,ansx,old_nid);
		}
		
	}

	//map<int,int>& next_map = nextLink.getMap();
	for(int nid =nextLink.getFirst();nid!=-1;nid=nextVal[nid].next){
	//for(it=next_map.begin();it!=next_map.end();it++){
		int tmp;
		int min_val = nextLink.getMin(tmp);
		int v = nextVal[nid].val+1;
		if(v>min_val+beam)continue;
		Node* p = tree[nid];
		int size = p->getSonSize();
		for(int i = 0;i<size;i++){
			int son_id = p->getSon(i)->getNid();
			bool f=nextLink.accept(son_id,v, column);
			if(f)path.create(ansx+1,son_id,ansx+1,nid);
		}
	}
}
