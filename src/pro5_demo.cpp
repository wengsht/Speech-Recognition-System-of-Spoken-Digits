#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include "resource.h"
#include "LexTree.h"
#include "spellcheck.h"
#include <iostream>

//#include "app.h"
//#include "conf.h"
using namespace std;

int dealOpts(int argc,char ** argv);
void run();

//int Conf::threshold = 3;
//int Conf::beam = 3;
//int Conf::info = 0;
//int Conf::debug = 0;

int main(int argc,char ** argv)
{
	if(! dealOpts(argc, argv))
		return 0;
	// !!! system 
	system("clear");	
	run();
	return 0;
}

//APP app;

const char * help = "\
					 usage : ./pro3_demo \n\
					 -h help. \n\
					 -a use beam pruning, also input a beam.\n\
					 -d use a dictionay to compare.\n\
					 -i output more information(for debug).\n\
					 -f save modified txt into a file.\n\
					 \n\
					 example:\n\
					 \n\
					 ";

///
int dealOpts(int argc,char ** argv){
	//	app.setFromFile(false);
	int c;	

	while((c = getopt(argc, argv,"ic:s:d:ha:A:mMf:")) != -1) 
	{
		//printf("%d\n",c);
		switch(c) {
			case 'i':
				//Conf::info = 1;
				break;
			case 'f':
				//app.setBStory(optarg);
				break;
			case 'd':
				//app.setFromFile(true);
				//app.setDict(optarg);
				break;
			case 'm':
				//app.setMode(2);
				break;
			case 'M':
				//app.setMode(3);
				break;
			case 'h':
				printf("%s",help);
				return false;
				break;
			case 'A':
				//app.setAlg(3);
				//Conf::beam = atoi(optarg);
				break;
			case 'a':
				//app.setAlg(2);
				//Conf::threshold = atoi(optarg);
				break;
			case 's':
				//app.setFromFile(true);
				//app.setStory(optarg);
				break;
			case 'c':
				//app.setFromFile(true);
				//app.setCStory(optarg);
				break;
			default:
				return false;
				break;
		}
	}

	//	app.setAlg(1);
	return true;	
}

void test(){
	//t.showInfo();
//	SpellChecker sp("mydict.txt",100);
	SpellChecker sp("dict_2.txt",100);
//	sp.showInfo();
	char c[1024] ;//= "daklashdfkjasdhfkjehwui";
	while(cin >> c){
		sp.check(c,false,true);
		//sp.check(c);
		sp.printAns();
//		printf("%d %s\n",tid,sp.getWord(tid));
	}
	printf("\n");
}

void run(){
	//app.setAlg(1);
	//app.setMode(1);
	//app.run();
	test();
}



