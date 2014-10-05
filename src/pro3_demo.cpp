#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include "resource.h"
#include "app.h"
#include "Conf.h"
using namespace std;

int dealOpts(int argc,char ** argv);
void run();

int Conf::threshold = 3;
int Conf::beam = 3;

int main(int argc,char ** argv)
{
	if(! dealOpts(argc, argv))
        return 0;

	// !!! system 
	system("clear");	
	run();
	return 0;
}

APP app;

const char * help = "\
usage : ./pro3_demo [-h] [-a|-A <NUM>] [-m|-M] \
[-d <FILE>] [-s <FILE>] [-c <FILE>]\n\
-h 帮助 \n\
-a 使用第一种剪枝(默认使用普通算法)，需要输入threshold \n\
-A 使用beam剪枝，需要输入beam \n\
-m 使用句子模式\n\
-M 使用文章模式\n\
-d 使用字典文件 \n\
-s 使用故事文本 \n\
-c 使用正确故事文本 \n\
";

///
int dealOpts(int argc,char ** argv){

	app.setFromFile(false);


	int c;	
    
	while((c = getopt(argc, argv, 
				"c:s:d:ha:A:mM")
			) != -1) 
	{
		//printf("%d\n",c);
        switch(c) {
			case 'd':
				app.setFromFile(true);
				app.setDict(optarg);
				break;
			case 'm':
				app.setMode(2);
				break;
			case 'M':
				app.setMode(3);
				break;
            case 'h':
        		Tip("%s",help); 
                return false;
				break;
			case 'A':
				app.setAlg(3);
				Conf::beam = atoi(optarg);
				break;
			case 'a':
				app.setAlg(2);
				Conf::threshold = atoi(optarg);
				break;
			case 's':
				app.setFromFile(true);
				app.setStory(optarg);
				break;
			case 'c':
				app.setFromFile(true);
				app.setCStory(optarg);
				break;
			default:
				return false;
		        break;
        }
	}
	
	app.setAlg(1);
	return true;	
}

void run(){
	app.setAlg(1);
	app.setMode(1);
	app.run();
}



