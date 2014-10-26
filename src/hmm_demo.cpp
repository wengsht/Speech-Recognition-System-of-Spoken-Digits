/*  TODO 进度条 */



// =====================================================================================
// 
//       Filename:  hmm_demo.cpp
// 
//    Description:  This is a demo code for HMM/ soft or kmean
// 
//        Version:  0.01
//        Created:  2014/10/21 21时32分22秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "resource.h"
#include "srs.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "FeatureExtractor.h"
#include "WaveFeatureOP.h"
#include <unistd.h>
#include "WaveFeatureOP.h"
#include "WordDtwRecognition.h"
#include <vector>
#include "tool.h"
#include "configure_dtw.h"
#include "HMMRecognition.h"
#include "ThreadPool.h"

using namespace std;

int  threadNum = DEFAULT_THREAD_NUM;
char templateFileName[1000] = "";
char inputFileName[1000] = "";

char templateDirName[256] = TEMPLATES_DIR;
char inputDirName[256] = INPUTS_DIR;

bool dealOpts(int argc, char **argv);

string getBanner(string word);

string algo = "kmean";

int gaussNum = GAUSSIAN_NUM;

// -g 0(default 1-1)
// -g 1(n-1)
// -g 2(n-n)
int demoType = 0;

void runNN();
void runN1();
int main(int argc, char **argv) {
    if(! dealOpts(argc, argv))
        return 0;

    if(demoType == 0)
        runN1();
    if(demoType == 1) 
        runNN();

//    cout << res << endl;

    return 0;
}
void runN1() {
    FeatureExtractor extractor(threadNum);

    RawData data;
    if(strlen(inputFileName) == 0) {
        Tip("[Capture an input!!]\n\n");
        capture("tmp", data, false);
    }
    else {
        Tip("[Load an input!!]\n\n");
        load_wav_file(inputFileName, data);
    }
    extractor.exDoubleDeltaFeatures(&data);
    vector<Feature> inputFeature = extractor.getNormalMelCepstrum();

    HMMRecognition hmm;
    hmm.loadTemplates(templateDirName);
    hmm.setGaussNum(gaussNum);
    
    if(algo == "kmean")
        hmm.setStateType(HMMState::KMEAN);
    else if(algo == "soft")
        hmm.setStateType(HMMState::SOFT);

    hmm.hmmTrain();

    WaveFeatureOP op(inputFeature, "null");
    string res = hmm.hmmRecognition(op);
    
    cout << getBanner(res) << endl;
    hmm.close();

}
void runNN() {
    FeatureExtractor extractor(threadNum);

    RawData data;

    HMMRecognition hmm;
    hmm.loadTemplates(templateDirName);
    hmm.setGaussNum(gaussNum);
    
    if(algo == "kmean")
        hmm.setStateType(HMMState::KMEAN);
    else if(algo == "soft")
        hmm.setStateType(HMMState::SOFT);

    hmm.hmmTrain();
    WaveFeatureOPSet inputs;
    inputs.loadMfccs(inputDirName);

//    WaveFeatureOP op(inputFeature, "null");
//    string res = hmm.hmmRecognition(op);
    
    WaveFeatureOPSet::iterator Itr = inputs.begin();
    
    int allCnt=0, correctCnt=0;
    for(; Itr != inputs.end(); Itr ++) {
        allCnt ++;

        string res = hmm.hmmRecognition(*(*Itr));
        
        if(res == (*Itr)->getWord())
            correctCnt ++;
    }
    system("clear");
    cout << "Input " << allCnt << " wavs " << endl; 
    cout << "Correct Counts: " << correctCnt << endl; 

    hmm.close();
}

bool dealOpts(int argc, char **argv) {
    int c;
    while((c = getopt(argc, argv, "k:a:g:bB:hj:d:t:D:T:")) != -1) {
        switch(c) {
            case 'h':
                printf("usage: \n \
                        filename example: abc\n \
                        -j threadNum \n \
                        -g demoType : 0(1-1) 1(n-1) 2(n-n)\n \
                        -k gaussNum \n \
                        -a [kmean, soft] \n \
                        -D input Dir name \n \
                        -T template Dir name\n \
                        -d input file name[capture if not set] \n \
                        -t template file name[capture if not set]\n");

                return false;
                break;
            case 'j':
                threadNum = atoi(optarg);
                ThreadPool::thread_num = threadNum;
                break;
            case 'g':
                demoType = atoi(optarg);
                break;
            case 'k':
                gaussNum = atoi(optarg);
                break;
            case 'T':
                strcpy(templateDirName, optarg);
                break;
            case 'D':
                strcpy(inputDirName, optarg);
                break;
            case 't':
                strcpy(templateFileName, optarg);
                break;
            case 'd':
                strcpy(inputFileName, optarg);
                break;
            case 'a':
                algo = string(optarg);
                break;
            default:
                break;
        }
    }
    return true;
}

string getBanner(string word) {

if("zero" == word) return " \n \
 ########   .####:    ##.####   .####.  \n \
 ########  .######:   #######  .######. \n \
     :##:  ##:  :##   ###.     ###  ###  \n \
    :##:   ########   ##       ##.  .##  \n \
   :##:    ########   ##       ##    ## \n \
  :##:     ##         ##       ##.  .## \n \
 :##:      ###.  :#   ##       ###  ### \n \
 ########  .#######   ##       .######. \n \
 ########   .#####:   ##        .####.  \n ";
                              
if("one" == word) return "  \n \
  .####.   ##.####    .####:  \n \
 .######.  #######   .######: \n \
 ###  ###  ###  :##  ##:  :##  \n \
 ##.  .##  ##    ##  ########  \n \
 ##    ##  ##    ##  ######## \n \
 ##.  .##  ##    ##  ##       \n \
 ###  ###  ##    ##  ###.  :# \n \
 .######.  ##    ##  .####### \n \
  .####.   ##    ##   .#####: \n ";
                              
if("two" == word) return "  \n \
   ##                         \n \
   ##                         \n \
 #######  ##      ##  .####.   \n \
 #######  ##.    .## .######.  \n \
   ##      #: ## :#  ###  ### \n \
   ##     :#:.##.:#: ##.  .## \n \
   ##      # :##:##  ##    ## \n \
   ##      ## ## ##  ##.  .## \n \
   ##.     ###::##   ###  ### \n \
   #####   :##..##:  .######. \n \
   .####   .##  ##    .####.  \n ";
                              
                              
                              
                              
                                                  
                                                  
if("three" == word) return "  \n \
           ##                                     \n \
   ##      ##                                     \n \
   ##      ##                                      \n \
 #######   ##.####    ##.####   .####:    .####:   \n \
 #######   #######    #######  .######:  .######: \n \
   ##      ###  :##   ###.     ##:  :##  ##:  :## \n \
   ##      ##    ##   ##       ########  ######## \n \
   ##      ##    ##   ##       ########  ######## \n \
   ##      ##    ##   ##       ##        ##       \n \
   ##.     ##    ##   ##       ###.  :#  ###.  :# \n \
   #####   ##    ##   ##       .#######  .####### \n \
   .####   ##    ##   ##        .#####:   .#####: \n ";

if("four" == word) return "  \n \
   :####                                \n \
   #####                                \n \
   ##                                    \n \
 #######    .####.   ##    ##   ##.####  \n \
 #######   .######.  ##    ##   ####### \n \
   ##      ###  ###  ##    ##   ###.    \n \
   ##      ##.  .##  ##    ##   ##      \n \
   ##      ##    ##  ##    ##   ##      \n \
   ##      ##.  .##  ##    ##   ##      \n \
   ##      ###  ###  ##:  ###   ##      \n \
   ##      .######.   #######   ##      \n \
   ##       .####.     ###.##   ##      \n ";
                                        
if("five" == word) return "  \n \
              ##                        \n \
   :####      ##                        \n \
   #####      ##                         \n \
   ##                                    \n \
 #######    ####     ##:  :##   .####:  \n \
 #######    ####      ##  ##   .######: \n \
   ##         ##     :##  ##:  ##:  :## \n \
   ##         ##      ##..##   ######## \n \
   ##         ##      ##::##   ######## \n \
   ##         ##      :####:   ##       \n \
   ##         ##       ####    ###.  :# \n \
   ##      ########    ####    .####### \n \
   ##      ########    :##:     .#####: \n ";
                              
if("six" == word) return "  \n \
              ##              \n \
              ##              \n \
              ##               \n \
                               \n \
  :#####.   ####     ###  ### \n \
 ########   ####      ##::##  \n \
 ##:  .:#     ##      :####:  \n \
 ##### .      ##       ####   \n \
 .######:     ##       :##:   \n \
    .: ##     ##       ####   \n \
 #:.  :##     ##      :####:  \n \
 ########  ########   ##::##  \n \
 . ####    ########  ###  ### \n ";
                                                  
if("seven" == word) return "  \n \
  :#####.   .####:   ##:  :##   .####:   ##.####  \n \
 ########  .######:   ##  ##   .######:  #######  \n \
 ##:  .:#  ##:  :##  :##  ##:  ##:  :##  ###  :##  \n \
 ##### .   ########   ##..##   ########  ##    ##  \n \
 .######:  ########   ##::##   ########  ##    ## \n \
    .: ##  ##         :####:   ##        ##    ## \n \
 #:.  :##  ###.  :#    ####    ###.  :#  ##    ## \n \
 ########  .#######    ####    .#######  ##    ## \n \
 . ####     .#####:    :##:     .#####:  ##    ## \n ";
                                                  
                                                  
                                                  
                                                  
                                                  
if("eight" == word) return "  \n \
              ##                                  \n \
              ##               ##                 \n \
              ##               ##          ##      \n \
                               ##          ##      \n \
  .####:    ####      :###:##  ##.####   #######  \n \
 .######:   ####     .#######  #######   #######  \n \
 ##:  :##     ##     ###  ###  ###  :##    ##     \n \
 ########     ##     ##.  .##  ##    ##    ##     \n \
 ########     ##     ##    ##  ##    ##    ##     \n \
 ##           ##     ##.  .##  ##    ##    ##     \n \
 ###.  :#     ##     ###  ###  ##    ##    ##.    \n \
 .#######  ########  .#######  ##    ##    #####  \n \
  .#####:  ########   :###:##  ##    ##    .####  \n \
                      #.  :##                     \n \
                      ######                      \n \
                      :####:                      \n ";
                                                  
                                        
if("nine" == word) return "  \n \
              ##                        \n \
              ##                        \n \
              ##                         \n \
                                         \n \
 ##.####    ####     ##.####    .####:  \n \
 #######    ####     #######   .######: \n \
 ###  :##     ##     ###  :##  ##:  :## \n \
 ##    ##     ##     ##    ##  ######## \n \
 ##    ##     ##     ##    ##  ######## \n \
 ##    ##     ##     ##    ##  ##       \n \
 ##    ##     ##     ##    ##  ###.  :# \n \
 ##    ##  ########  ##    ##  .####### \n \
 ##    ##  ########  ##    ##   .#####: \n";

if("ten" == word) return "  \n \
   ##                         \n \
   ##                         \n \
 #######    .####:   ##.####   \n \
 #######   .######:  #######   \n \
   ##      ##:  :##  ###  :## \n \
   ##      ########  ##    ## \n \
   ##      ########  ##    ## \n \
   ##      ##        ##    ## \n \
   ##.     ###.  :#  ##    ## \n \
   #####   .#######  ##    ## \n \
   .####    .#####:  ##    ## \n ";

return "null2";
}
