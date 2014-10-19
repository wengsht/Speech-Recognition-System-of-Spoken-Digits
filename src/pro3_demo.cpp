#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include "resource.h"
#include "app.h"
#include "conf.h"
using namespace std;

int dealOpts(int argc,char ** argv);
void run();

int Conf::threshold = 3;
int Conf::beam = 3;
int Conf::info = 0;
int Conf::debug = 0;
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
[-d <FILE>] [-s <FILE>] [-c <FILE>] [-f <FILE>]\n\
-h help. \n\
-a use a fix threshold (default use nothing), also input a threshold.\n\
-A use beam pruning, also input a beam.\n\
-m compare two sentence (default compare words).\n\
-M compare two article.\n\
-d use a dictionay to compare.\n\
-s use one story txt.\n\
-c use one correct story txt, always use with -s together.\n\
-i output more information(for debug).\n\
-f save modified txt into a file.\n\
\n\
example:\n\
-a 3 \n\
--- input two words to compare with fix threshold 3.\n\
\n\
-m -a 3 \n\
--- input two sentence to compare with beam 3.\n\
\n\
-s a1.txt -c b1.txt -M\n\
--- compare two files. calculate lines' different.\n\
\n\
-d dict.txt\n\
--- input one word, and compare it with the whole dictionary.\n\
\n\
-c storycorrect.txt -s story.txt -d dict.txt -f better.txt\n\
--- modify story.txt with dict.txt, and compare with storycorrect.txt.\n\
then save modified story into better.txt\n\
\n\
-s a.txt -c b.txt -m\n\
--- compare two files, calculate words' different.\n\
\n\
";

///
int dealOpts(int argc,char ** argv){

	app.setFromFile(false);


	int c;	
    
	while((c = getopt(argc, argv, 
				"ic:s:d:ha:A:mMf:")
			) != -1) 
	{
		//printf("%d\n",c);
        switch(c) {
			case 'i':
				Conf::info = 1;
				break;
			case 'f':
				app.setBStory(optarg);
				break;
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



