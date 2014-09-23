#include "srs.h"
#include <vector>
#include <string>
#include <iostream>
#include <string.h>
//#include "resource.h"
#include <ctype.h>
#include <stdio>

using namespace std;
vector<string> w;

int main()
{
	printf("project3\n");
	if(getStrsFromFile(&w,"story.txt",".,:!?\"",true)){
		for(int i = 0;i<w.size();i++){
			cout << w[i] << " ";
		}
		puts("");
	}
	return 0;
}
