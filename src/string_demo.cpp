#include "srs.h"
#include <vector>
#include <string>
#include <iostream>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

using namespace std;
vector<string> w;

void test(){
	if(getStrsFromFile(&w,"story.txt",".,:!?\"",true)){
		for(int i = 0;i<w.size();i++){
			cout << w[i] << " ";
		}
		puts("");
	}	
}

void test2(){
	string a,b;
	cin >> a >> b;
	StringMatch sm;

	int ans = sm.compare(a.c_str(),b.c_str());
	printf("%d\n",ans);
}

void test3(){	
	StringMatch sm;
	sm.run("dict.txt","story.txt","newStory.txt");
}
int main(int arg,char ** argv)
{
	if(arg>1){
		test3();
	}
	else{
		test2();
	}
	return 0;
}
