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
//	system("clear");	
	run();
	return 0;
}

//APP app;

const char * help = "\
usage : ./pro3_demo \n\
-h help. \n\
-b use beam pruning,  input a beam(default = INF).\n\
-d use a dictionay to compare.\n\
-i output more information(for debug).\n\
-m deal with multi words without blank.\n\
\n\
example:\n\
./pro5_demo -d dict_2.txt -m -b 5 < unsegmented0.txt\n\
";

///

char dictFile[1024];
char txtFile[1024];
char correctFile[1024];
int beam;
int one;

int dealOpts(int argc,char ** argv){
	//	app.setFromFile(false);
	int c;	
	beam = 0x3f3f3f3f;
	dictFile[0]='\0';
	txtFile[0]='\0';
	correctFile[0]='\0';
	one = true;

	while((c = getopt(argc, argv,"id:hb:mf:")) != -1) 
	{
		//printf("%d\n",c);
		switch(c) {
			case 'i':
				//Conf::info = 1;
				break;
			case 'f':
				strcpy(txtFile,optarg);
				break;
			case 'd':
				strcpy(dictFile,optarg);
				break;
			case 'm':
				one = false;
				break;
//			case 'M':
//				//app.setMode(3);
//				break;
			case 'h':
				printf("%s",help);
				return false;
				break;
//			case 'A':
//				//app.setAlg(3);
//				//Conf::beam = atoi(optarg);
//				break;
			case 'b':
				beam = atoi(optarg);
				//app.setAlg(2);
				//Conf::threshold = atoi(optarg);
				break;
//			case 's':
//				//app.setFromFile(true);
//				//app.setStory(optarg);
//				break;
//			case 'c':
//				//app.setFromFile(true);
//				//app.setCStory(optarg);
//				break;
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
		//sp.check(c,false,true);
		sp.check(c);
	//	sp.printAns();
//		printf("%d %s\n",tid,sp.getWord(tid));
	}
	printf("\n");
}

void run(){
	//app.setAlg(1);
	//app.setMode(1);
	//app.run();
//	test();
	SpellChecker sp(dictFile,beam);
	char c[1024];
	while(cin>>c){
		sp.check(c,one);
		sp.printAns();
	}
	printf("\n");
}



