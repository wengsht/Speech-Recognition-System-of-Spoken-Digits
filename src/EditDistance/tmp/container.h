#ifndef __PRO3__EDITDISTANCE__CONTAINER__
#define __PRO3__EDITDISTANCE__CONTAINER__
#include "tool.h"
#include <vector>
using namespace std;
// 编辑距离的计算单元，即“单词”
// T为编辑距离中的“字母”
// T必须定义或重载== 和 = 

template <typename T>
class Container{
protected:
	vector<T> items;
public:
	Container(){}
	
	Container(T* items,int len){
		init(items,len);
	}

	~Container(){}
	
	//检查第i位是不是元素a
	bool check(int i,const T& a)const{
		return items[i] == a;
	}
	int len()const{return items.size();}
	void add(const T& t){items.push_back(t);}
	const T& operator [](int t)const{return items[t];}

	friend bool operator <(
					const Container<T>& a,
					const Container<T>& b)
	{
		if(a.len != b.len)return a.len < b.len;
		for(int i =0;i<a.len;i++){
			if(a.items[i] != b.items[i]){
				return a.items[i] < b.items[i];
			}
		}
		return false;
	}
	
	friend bool operator ==(
					const Container<T>& a,
					const Container<T>& b)
	{
		if(a.len != b.len)return false;
		for(int i =0;i<a.len;i++){
			if(a.items[i] != b.items[i]){
				return false;
			}
		}
		return true;
	}
};

#endif
