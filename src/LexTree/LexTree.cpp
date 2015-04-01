#include "LexTree.h"
#include "stringtool.h"
#include <string.h>
#include <algorithm>
#include <map>
#include <iostream>
#include <stdio.h>
#include <queue>

using namespace std;


LexTree::LexTree(){
	root = new Node();
	nodes.push_back(root);
} 
 
LexTree::~LexTree(){
	if (root) delete root;
	root  = NULL;
} 

map<string,int> p;
bool cmp(const string& a,const string& b){
	return p[a]>p[b];
}
void LexTree::buildTree(const char * fileName){
	vector<string> words;
	getStrsFromFile(words,fileName,"\r\n");
	/*string s;
	int d;
	while(cin>>s>>d){
		transform(s.begin(), s.end(), s.begin(), ::tolower);
		p[s]+=d;	
	}

	printf("start\n");
	sort(words.begin(),words.end(),cmp);
	for(int i = 0;i<words.size();i++){
		cout << words[i] << endl;
	}
	*/
	this->buildTree(words);
}



void LexTree::add(const char * word){
	int len = strlen(word);
	Node * nowNode = this->root;
	int size = nodes.size();
	for(int j = 0;j<len-1;j++){
		nowNode	= nowNode->findOrAddson(word[j],false,size,NULL);		
		if(size != nodes.size()){
			nodes.push_back(nowNode);
		}
	}
	
	nowNode = nowNode->findOrAddson(word[len-1],true,size,word);
	
	if(size != nodes.size()){
		nodes.push_back(nowNode);
	}

//	leaf.insert(nowNode);
}

void LexTree::buildTree(const vector<string> &  words){
	int size = words.size();
	for(int i = 0;i<size ;i++){
		const char * w = words[i].c_str();
		this->add(w);
	}
	//BFSNodes();
}

void LexTree::showInfo(){
	printf("Nodes: %lu\n",nodes.size());
/*	for(int i = 0;i<nodes.size();i++){
		Node * p = nodes[i]->getPnt();
		if(p==NULL)printf("-");
		else printf("%d",p->getNid());
		printf(" %c\n",nodes[i]->getC());
	}
*/
}

void LexTree::BFSNodes(){
	int cnt = 0;
	queue<Node*> q;
	q.push(root);
	while(!q.empty()){
		Node * tmp = q.front();
		q.pop();
		tmp->setNid(cnt);
		nodes[cnt++] = tmp;
		int size = tmp->getSonSize();
		for(int i =0;i<size;i++){
			Node * node = tmp->getSon(i);
			if(node != root)
				q.push(node);
		}
	}
}
