#ifndef __SpeechRecongnitionSystem__StringTool__
#define __SpeechRecongnitionSystem__StringTool__
#include <vector>
#include <string>
//using namespace std;

// words: an array of string read from a file
// splitor:
// blank: if use the blank (' ','\t','\n') as splitors
// return true when read success.
bool getStrsFromFile(std::vector<std::string>* words,const char * FileName, const char * splitor, bool blank = true);
bool  saveStrsToFileByTemplate(std::vector<std::string>* words,const char * saveFileName,const char* templateFileName, const char * splitor, bool blank=true);
#endif
