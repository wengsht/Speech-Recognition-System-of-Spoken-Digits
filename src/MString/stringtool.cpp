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
        ErrorLog("read file error: %s ",FileName);
		return false;
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
	if(str!=""){
		words->push_back(str);
	}
    return true;
}


bool  saveStrsToFileByTemplate(vector<string>* words,const char * saveFileName,const char* templateFileName, const char * splitor, bool blank)
{
    FILE * fptr;
	FILE * tptr;
    fptr = fopen(saveFileName,"w");
	tptr = fopen(templateFileName,"r");
    if(!fptr) {
        ErrorLog("read save file error: %s ",saveFileName);
		return false;
	}

    if(!tptr) {
        ErrorLog("read template file error: %s ",templateFileName);
		return false;
	}
    
	char c;
	int now = 0;
	int flag = 1;

    while(fscanf(tptr,"%c",&c)!=EOF)
    {
        c = tolower(c);
        if(charInStr(c, splitor) || (blank && charInStr(c,blankChar))){
			fprintf(fptr,"%c",c);
			flag = 1;
		}
        else{
			if(flag){
				if(now<words->size()){
					string tmp = (*words)[now];
					fprintf(fptr,"%s",tmp.c_str());
					flag = 0;
					now++;
				}
				else{
					ErrorLog("words are less than those in the template");
					return false;
				}
			}
        }
    }

    return true;
}





