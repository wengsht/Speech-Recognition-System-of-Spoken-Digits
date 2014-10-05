#include <cstring>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include "resource.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>
#include "stringtool.h"
using namespace std;

bool charInStr(const char& c, const char* str){
	int len  = strlen(str);
	for(int i = 0;i<len;i++){
		if(str[i] == c) return true;
	}
	return false;
}

string getStrsFromFile(const char *FileName)
{
	ifstream t(FileName);
	string f((std::istreambuf_iterator<char>(t)),  
                 std::istreambuf_iterator<char>()); 
	t.close();
	return f;
}

bool  getStrsFromFile(
		vector<string>& words,
		const char * FileName, 
		const char * splitor,
		bool lower,
		bool cleanBlank
		)
{
	string f  = getStrsFromFile(FileName);
	if(lower)
		transform(f.begin(), f.end(), f.begin(), ::tolower);

	int len = f.size();
	int st = 0;
	int l = 0;
	for(int i=0;i<len;i++){
		if(charInStr(f[i],splitor)){
			if(l || !cleanBlank){
				string t (f.c_str()+st,l);
				words.push_back(t);
				l=0;
			}
			st=i+1;
		}
		else{l++;}
	}

	if(l || !cleanBlank){
		string t (f.c_str()+st,l);
		words.push_back(t);
	}
    return true;	
}

