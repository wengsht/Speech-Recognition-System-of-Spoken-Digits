#ifndef __SpeechRecongnitionSystem__StringMatch__
#define __SpeechRecongnitionSystem__StringMatch__
#include "tool.h"
#include <string>
#include <vector>
typedef char MCHAR ;
typedef std::pair<int,int> pii;

#define mp make_pair

class StringMatch{
protected:
	int MAX_DIFF;
	std::vector<std::string> dict;
	std::vector<std::string> words;
	std::vector<std::string> new_words;
	void print(std::vector<std::string> * v);
	bool saveNewWordsToFileWithTemplate(const char * saveFileName,const char * templateFileName);
	int words_len;
	int dict_len;
	int * buffer;
	int * path;

    int get(pii p){return get(p.first,p.second);}

    int renew(pii p,int v){return renew(p.first,p.second,v);}

    int get(int x,int y){
        return buffer[x*dict_len+y];
    }
    // -1 before renew is equal -1
    // 0 renew and is the same
    // 1 renew and smaller
    int renew(int x,int y ,int v){
        int p = x*dict_len+y;
        if(buffer[p] == -1){
            buffer[p] = v;
            return -1;
        }
        else{
            if(buffer[p] > v){
            	buffer[p] = v;
                return 1;
            }
        }
        return 0;
    }

	void initbuffer(){
		for(int i = 0;i<(words_len + 1) * (dict_len + 1);i++){
			buffer[i] = -1;
		}
	}
public:
	StringMatch(){
		MAX_DIFF = 100;
		buffer = NULL;
		path = NULL;
	}

	~StringMatch(){
		SAFE_DELETE_POINTER(buffer);
		SAFE_DELETE_POINTER(path);
	}

	int initDict(const char * FileName);
	int initWords(const char * FileName);
	int compare(const MCHAR * in,const MCHAR * tp);		
	void run(const char * dictFile,
			const char * wordFile,
			const char * saveFile);
};

#endif
