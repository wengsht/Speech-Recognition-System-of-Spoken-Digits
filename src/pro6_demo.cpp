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

#include "parseGraph.h"
#include <iostream>
#include <unistd.h>
#include <iomanip>
#include <cstdio>
#include <cstdlib>
#include "RawData.h"
#include "HMMSeqRecognition.h"
#include "FeatureExtractor.h"
#include "ThreadPool.h"
#include <fstream>
#include "test.h"
#include "Capture.h"
#include "SeqModel.h"
#include "configure_hmm.h"

using namespace std;

bool dealOpts(int argc, char **argv);

//char tempDir[100] = "./combine";
char inputDir[100] = "./combine2";
char tempDir[100] = "./combine";
char inputFileName[100] = "";

const char seg[] = "seg";
const char seq[] = "seq";
char trainType[100] = "seg";

int threadNum = 4;
double beam;
bool doBeam = false;

char graphPath[1000] = "./Graph/phone_number.graph";

int gaussNum = GAUSSIAN_NUM;

SeqModel::SEQ_DTW_PATH_TYPE pathType = SeqModel::BACK_PTR;

void runN1();
void runNN();

int mode = 0;

int main(int argc, char **argv) {
    if(! dealOpts(argc, argv))
        return 0;

    if(0 == mode)
        runN1();
    else 
        runNN();
    return 0;
}

void runNN() {
    HMMSeqRecognition recognition;

    recognition.setStateType(HMMState::KMEAN);
    recognition.setGaussNum(gaussNum);

    if(recognition.hmmTryLoad(tempDir) == SP_SUCCESS) {
    }
    else {
        recognition.loadTemplates(tempDir);

        if(0 == strcmp(trainType, seq)) {
            recognition.hmmSeqTrain();
        }
        else 
            recognition.hmmSegTrain();
    }

    recognition.loadGraph(graphPath); //"./Graph/phone_number.graph");

    FeatureExtractor extractor(threadNum);

    /*  
    RawData data;
    if(strlen(inputFileName) == 0) {
        Tip("[Capture an input!!]\n\n");
        capture("tmp", data, true);
    }
    else {
        Tip("[Load an input!!]\n\n");
        Capture::load_wav_file(inputFileName, data);
    }

    extractor.exDoubleDeltaFeatures(&data);
    vector<Feature> inputFeature = extractor.getNormalMelCepstrum();

    WaveFeatureOP op(inputFeature, "null");
    */

    if(doBeam)
        recognition.setBeam(beam);

    vector<string> res;

    WaveFeatureOPSet inputs;
    inputs.loadMfccs(inputDir);

    WaveFeatureOPSet::iterator Itr = inputs.begin();

    for(; Itr != inputs.end(); Itr ++) {
        res.clear();

        recognition.recognition(*(*Itr), res, pathType);

        cout << setw(20) << left << " Input: " <<  (*Itr)->getWord() <<  " " << (*Itr)->getFileName() << endl;
        cout << setw(20) << left << "Templates: ";
        for(int i = 0;i < res.size(); i++) {
            cout << RED << res[i] << " " ;
        }
        puts("");
        cout << NONE << endl;
    }

    ofstream dotOut("model.dot");
    recognition.dumpDot(dotOut);
    dotOut.close();

    recognition.close();

    /*  
        for(int i = 0;i < res.size(); i++) {
        cout << RED << res[i] << " " ;
        }
        puts("");
        cout << NONE;
        */
}
void runN1() {
    HMMSeqRecognition recognition;

    recognition.setStateType(HMMState::KMEAN);
    recognition.setGaussNum(gaussNum);

    if(recognition.hmmTryLoad(tempDir) == SP_SUCCESS) ;
    else {
        recognition.loadTemplates(tempDir);
        if(0 == strcmp(trainType, seq))
            recognition.hmmSeqTrain();
        else 
            recognition.hmmSegTrain();
    }

    recognition.loadGraph(graphPath); //"./Graph/phone_number.graph");

    FeatureExtractor extractor(threadNum);

    RawData data;
    if(strlen(inputFileName) == 0) {
        Tip("[Capture an input!!]\n\n");
        capture("tmp", data, true);
    }
    else {
        Tip("[Load an input!!]\n\n");
        Capture::load_wav_file(inputFileName, data);
    }

    extractor.exDoubleDeltaFeatures(&data);
    vector<Feature> inputFeature = extractor.getNormalMelCepstrum();
    WaveFeatureOP op(inputFeature, "null");
    vector<string> res;

    if(doBeam)
        recognition.setBeam(beam);
    recognition.recognition(op, res, pathType);

    ofstream dotOut("model.dot");
    recognition.dumpDot(dotOut);
    dotOut.close();

    recognition.close();

    for(int i = 0;i < res.size(); i++) {
        cout << RED << res[i] << " " ;
    }
    puts("");
    cout << NONE;
}

bool dealOpts(int argc, char **argv) {
    int c;
    while((c = getopt(argc, argv, "hd:p:g:b:G:T:D:t:m:")) != -1) {
        switch(c) {
            case 'h':
                printf("usage: \n \
                        -h \n \
                        -d inputwav \n\
                        -g gaussNum \n\
                        -G graph file path \n\
                        -T templateDir \n\
                        -D inputDir \n\
                        -b beam_value \n\
                        -m 0:runN1 1:runNN\n\
                        -t seg/seq : seg / sequential train \n\
                        -p backptr/full : backPtr/fullPath\n");

                return false;
                break;
            case 'd':
                strcpy(inputFileName, optarg);
                break;
            case 't':
                strcpy(trainType, optarg);
                break;
            case 'b':
                doBeam = true;
                beam = atof(optarg);
                break;
            case 'T':
                strcpy(tempDir, optarg);
                break;
            case 'D':
                strcpy(inputDir, optarg);
                break;
            case 'g':
                gaussNum = atoi( optarg );
                break;
            case 'm':
                mode = atoi( optarg );
                break;
            case 'G':
                strcpy(graphPath, optarg);
                break;
            case 'p':
                if(0 == strcmp(optarg, "backptr"))
                    pathType = SeqModel::BACK_PTR;
                else if(0 == strcmp(optarg, "full"))
                    pathType = SeqModel::FULL_PATH;

                break;

            default:
                break;
        }
    }
    return true;
}

