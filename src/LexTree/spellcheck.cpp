#include "spellcheck.h"
#include <string.h>
#include <string>

using namespace std;

SpellChecker::SpellChecker(){
	beam = INF;
}

SpellChecker::~SpellChecker(){
	
}

int SpellChecker::checkOneWord(const char * word){
	Link * list = initLink();
	int now = 0;
	int len = strlen(word);
	for(int i = 0;i<len;i++){
		refreshLink(word[i],list[now],list[now^1],i==(len-1));
		list[now].clear();
		now ^=1;
//		printf("after %d update: \n",i);
//		list[now].print();
	//	printf("list size = %lu\n\n",list[now].getMap().size());
	}
	int index,val;
	val = list[now].getMin(index);
//	printf("min_val=%d,  word:%s, find word=%s\n",val,word,getWord(index));	
	printf("%s ",getWord(index));
	delete [] list;
	return index;
}

// for check one word
Link * SpellChecker::initLink(){
	Link * list = new Link[2];
	list[0].init(&tree,beam);
	list[1].init(&tree,beam);
	//list[0].print();
	//!!!
	list[0].setFirstLink();
//	list[0].print();
	return list;
}

//for check one word
//update next link, and next link update itself (remove node with beam)
void SpellChecker::refreshLink(char next_c,
								Link& nowLink,
								Link& nextLink,
								bool last )
{
	map<int ,int>::iterator it;
	map<int,int> now_map = nowLink.getMap();
	for(it = now_map.begin();it!=now_map.end();it++){
		int node_val = it->second;
		int old_nid = it->first;

		// delete the letter
		// !!!
		nextLink.accept(old_nid,node_val+1);
		
		Node * node = tree[old_nid];

		int size = node->getSonSize();
		
		for(int j=0;j<size;j++){
			Node * son = node->getSon(j);
			int new_nid = son->getNid();

			if(next_c==son->getC()){// match the letter
				nextLink.accept(new_nid,node_val);		
			}
			else{// change the letter
				nextLink.accept(new_nid,node_val+1);		
			}
		}
		
	}
	//!!!
	nextLink.checkSelf(last);
}

/*
// for check a Text
int checkText(const string& text)
{
	
}
*/
