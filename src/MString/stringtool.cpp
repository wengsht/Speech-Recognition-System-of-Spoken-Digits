#include "stringtool.h"

#include <string.h>
#include <vector>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include "resource.h"

using namespace std;

bool charInStr(const char c, const char* str){
	int len  = strlen(str);
	for(int i = 0;i<len;i++){
		if(str[i] == c) return true;
	}
	return false;
}

const char * blankChar = " \t\n\r";


bool  getStrsFromFile(vector<string>* words,const char * FileName, const char * splitor, bool blank)
{
    FILE * fptr;
    fptr = fopen(FileName,"r");
    if(!fptr) {
 //       ErrorLog("read file error: %s ",FileName);
    }
    char c;
    string str;
    while(fscanf(fptr,"%c",&c)!=EOF)
    {
        c = tolower(c);
        if(charInStr(c, splitor) || (blank && charInStr(c,blankChar))){
            if(str=="")continue;
            words->push_back(str);
            str = "";
         }
        else{
            str += c;
        }
    }
    return true;
}





