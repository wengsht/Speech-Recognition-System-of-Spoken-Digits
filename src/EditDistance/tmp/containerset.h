#ifndef __PRO3__EDITDISTANCE__CONTAUNERSET__
#define __PRO3__EDITDISTANCE__CONTAINERSET__
#include "tool.h"
#include "Container.h"
#include <vector>
#include <set>

using namespace std;

template <typename T>
class ContainerSet
{
private:
	set< Container<T> > Text;
	vector <int> Key;
public:

	virtual bool readFromKeyBoard(){
		Warn("Can't use keyboard");
		return false;
	};

	virtual bool readFromFile(const char * file_name)=0;

	virtual bool corectWithDic(const char * file_name){
		Warn("Can't use dictionary!");
		return false;
	}

	virtual bool corectWithBKTree(const char * file_dic, 
					const char * file_bk){
		Warn("Can't use kb tree");		
		return false;
	}
};

#endif
