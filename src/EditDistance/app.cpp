#include "app.h"
#include "stringtool.h"
#include <set>
#include <map>
#include "calcDist.h"
#include "ThreadPool.h"
#include <iostream>

using namespace std;

 //各种模式分割符
const char * wordSp = ".,:!?\"\t\n\r ";
const char * sentenceSp = ".!? ";
const char * articleSp = "\n\r";

APP::APP(){
	alg=0;
	dictFile = NULL;
	betterStoryFile = NULL;
	storyFile = NULL;
	cStoryFile = NULL;
	mode = 0;
//	Conf::threshold = 4;
}
APP::~APP(){}


void APP::readFile(){
	if(dictFile != NULL && mode == 1){
		getStrsFromFile(dic,dictFile,"\n\r",true);
	}
	if(storyFile != NULL){
		if(mode == 1){
			getStrsFromFile(story,storyFile,wordSp,true);
			if(cStoryFile)getStrsFromFile(cStory,cStoryFile,wordSp,true);
		}
		else if(mode == 2){
			getStrsFromFile(story,storyFile,sentenceSp,false);
			if(cStoryFile)getStrsFromFile(cStory,cStoryFile,sentenceSp,false);
		}
		else if(mode == 3){
			getStrsFromFile(story,storyFile,articleSp,false,false);
			if(cStoryFile)getStrsFromFile(cStory,cStoryFile,articleSp,false,false);
		}
	}
}

void APP::print(const vector<string>& v,bool line)const{
	for(int i = 0;i<v.size();i++){
		printf("%s",v[i].c_str());
		if(line)printf(" ");
		else printf("\n");
	}
}

void APP::print(const vector<int>& index,const vector<string>& v,bool line)const{
	for(int i = 0;i<index.size();i++){
		int tmp = index[i];
		if(tmp<0 || tmp>=v.size()){
			Warn("index error when print");
		}
		printf("%s",v[tmp].c_str());
		if(line)printf(" ");
		else printf("\n");
	}	
}

bool APP::setMode(int t){
	if(mode !=0)return false;
	mode = t;
	return true;
}
bool APP::setAlg(int t){
	if(alg!=0)return false;
	alg = t;
	return true;
}

void APP::calcIndex(const vector<string>& org,
				vector<int>& index
)
{
	map<string,int> m;
	int size = org.size();
	index.resize(size);
	for(int i =0;i<size;i++){
		string t = org[i];
		int val = m[t];
		if(val==0){
			m[t] = i+1;
			index[i] = i;
		}
		else{
			index[i] = val - 1;
		}
	}	
}
void APP::printInfo(){
	printf("Algorithm : ");
	if(alg==1){
		puts("Normal");
	}
	else if(alg == 2){
		printf("Threshold of %d\n",Conf::threshold);
	}
	else if(alg == 3){
		printf("Beam of %d\n",Conf::beam);
	}
	printf("Compare obj : ");
	
	if(mode == 1){
		printf("Word-letter\n");
	}
	else if(mode == 2){
		printf("Sentence-word\n");
	}
	else if(mode == 3){
		printf("Article-line\n");
	}

	printf("From File : %s\n",fromFile?"Yes":"No");
}

bool APP::run(){
	printInfo();

	if(fromFile){
		return fromFileJob();
	}
	else{
		return inputJob();
	}

	return true;
}

void APP::cmp_task(void * in){
	cmp_task_info * task_info = (cmp_task_info *) in;
	vector<int>& result = *(task_info->result);
	
	result[task_info->index] = 
		compareOneWithDict(
			(task_info->w),
			*(task_info->dict),
			task_info->type
		);
	delete task_info;
}

int APP::calcOne(){
	int ret;
	puts("start calc");
	if(mode == 1){
		if(story.size()==0 || cStory.size()==0){
			Warn("empty story file or cStory file");	
			return false;
		}	
		
		if(alg == 1) ret = compare(story[0].c_str(),cStory[0].c_str());
		else if(alg == 2) ret = compare(story[0].c_str(),
				cStory[0].c_str(),Conf::threshold);
		else if(alg == 3) ret = 0;
		return ret;
	}

	if(alg == 1) ret = compare(story,cStory);
	else if(alg == 2) ret = compare(story,cStory,Conf::threshold);
	else if(alg == 3) ret = 0;
	return ret;
}

int APP::calcAllWithThread(){
    int st_l = story.size();
	better.resize(st_l);
	ThreadPool threadPool(thread_num);
    for(int i =0;i<st_l;i++){
		int ti = storyIndex[i];
		if(ti!=i){continue;}
		struct sp_task task_struct;
		struct cmp_task_info *task_info = new cmp_task_info;
		task_info->index = i;
		task_info->w =  story[i].c_str();
		task_info->dict = &dic;
		task_info->result = &better;
		task_info->type = alg;
		
		task_struct.func = cmp_task;
		task_struct.in   = task_info;
		threadPool.addTask(task_struct);
	//	better[i] = compareOneWithDict(story[i].c_str(),dic,alg);
    }
	threadPool.run();
	
	for(int i =0;i<st_l;i++){
		printf("storyIndex[%d] =  %d\n",i,storyIndex[i]);
		int ti = storyIndex[i];
		if(ti!=i){better[i] = better[ti];}
	}	
}

bool APP::fromFileJob(){
	readFile();
//	print(story,false);
	//puts("|||||||");
	//print(cStory,false);
	
	if(dic.size()==0){
		int result = calcOne();
		cout << "result : "<<result << endl;
		return true;
	}

	
	calcIndex(story,storyIndex);

	calcAllWithThread();

	print(better,dic,true);
	printf("\n\n");
	return true;
}

bool readline(vector<string>& v){
	string s="";
	char c;
	while(scanf("%c",&c)!=EOF){
		if(c == '\n'){
			if(s!=""){
				v.push_back(s);
			}
			s="";
			break;
		}
		else if(c==' '){
			if(s!="")v.push_back(s);
			s="";
		}
		else s= s+c;
	}
	if(v.size()==0)return 0;
	return 1;
}

bool APP::inputJob(){
	if(mode == 1){
		string a,b;
		while(cin >> a >> b){
			int ret ;
			if(alg==1) ret = compare(a.c_str(),b.c_str());
			if(alg==2) ret = compare(a.c_str(),b.c_str(),Conf::threshold);
			cout << ret << endl;
		}
	}
	else if(mode == 2){
		if(readline(story) && readline(cStory)){
			int ret ;
			if(alg==1) ret = compare(story,cStory);
			if(alg==2) ret = compare(story,cStory,Conf::threshold);
			cout << ret << endl;
		}
		else{
			Warn("Input error\n");
		}
	}
	else if(mode == 3){
		Warn("Can't not input two article, use File -s and -c!");
	}
	return true;
}
