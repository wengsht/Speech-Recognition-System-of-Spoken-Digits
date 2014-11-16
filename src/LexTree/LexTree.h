#ifndef __PRO5__LEXTREE__LEXTREE_H__
#define __PRO5__LEXTREE__LEXTREE_H__

#include <vector>
#include <set>
#include <string.h>
#include <string>

class Node{
private:
	// struct
	std::vector<Node *> sons;
	Node * pnt;
	// data
	bool is_leaf;
	char c;
	int nid;
	char * word;
public:
	Node(){
		pnt = NULL;
		is_leaf = false;
		c='*';
		nid=0;
		word = NULL;
	}

	Node(Node * pnt,char c,bool is_leaf,int nid,const char * w){
		this->pnt = pnt;
		this->is_leaf = is_leaf;
		this->c = c;
		this->nid = nid;
		if(w){
			word = new char[strlen(w)+1];
			strcpy(word,w);
		}
		else word = NULL;
	}
	// also delete sons
	~Node(){
		if(word)delete [] word;
		word=NULL;
		pnt=NULL;
		int size = sons.size();
		for(int i = 0;i<size;i++){
			if(sons[i])delete sons[i];
		}
		sons.clear();
	}

	Node* getPnt(){
		return pnt;
	}

	int getNid(){
		return nid;
	}
	void setNid(int nid){
		this->nid = nid;
	}
	char getC(){
		return c;
	}

	char * getWord(){
		return word;
	}


	// check the value and if is a leaf
	bool check(char c,bool is_leaf){
		if(this->c == c && this->is_leaf == is_leaf){
			return true;
		}
		return false;
	}

	// find a son with value c and leafflag is_leaf
	// if not exist, create one
	// return the son Node;
	Node* findOrAddson(char c,bool is_leaf,int& nid,const char * w){
		int size = sons.size();
		for(int i =0;i<size;i++){
			if(sons[i]->check(c,is_leaf)){
				return sons[i];
			}		
		}
		Node *t = new Node(this,c,is_leaf,nid,w);
		nid++;
		sons.push_back(t);
		return t;
	}
	bool isLeaf(){
		return is_leaf;
	}
	Node* getParent(){
		return pnt;
	}
	int getSonSize(){
		return sons.size();
	}
	Node* getSon(int index){
		return sons[index];
	}
};

class LexTree{
private:
	Node * root;
//	std::vector <std::string> words;
	std::vector<Node *> nodes;
protected:
	void add(const char * word);
//	void buildTree();
public:	
	LexTree();
	void buildTree(const char * fileName);
	void buildTree(const std::vector<std::string>& v);
	~LexTree();
	int getNodeSize(){
		return nodes.size();
	};
	void BFSNodes();
	void showInfo();
	Node* operator [](int i){
		if(i<0 || i>=nodes.size())return NULL; 
		return nodes[i];
	}
};


#endif
