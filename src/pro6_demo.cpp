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
#include "RawData.h"
#include "HMMSeqRecognition.h"
#include "FeatureExtractor.h"
#include "ThreadPool.h"
#include "test.h"
#include "Capture.h"

using namespace std;

bool dealOpts(int argc, char **argv);

char tempDir[100] = "./combine";
char inputFileName[100] = "";
int threadNum = 4;

int main(int argc, char **argv) {
    if(! dealOpts(argc, argv))
        return 0;

    HMMSeqRecognition recognition;

    recognition.setStateType(HMMState::KMEAN);

    if(recognition.hmmTryLoad(tempDir) == SP_SUCCESS) ;
    else {
        recognition.loadTemplates(tempDir);
        recognition.hmmSegTrain();
    }

//    recognition.loadTemplates(tempDir);

//    recognition.hmmSegTrain();

    recognition.loadGraph("./Graph/phone_number.graph");

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

//    recognition.setBeam(1000);
    recognition.recognition(op, res);

    recognition.close();

    for(int i = 0;i < res.size(); i++) {
        cout << res[i] << " " ;
    }
    puts("");

//    cout << res << endl;

    return 0;
}

bool dealOpts(int argc, char **argv) {
    int c;
    while((c = getopt(argc, argv, "hd:")) != -1) {
        switch(c) {
            case 'h':
                printf("usage: \n \
                        -h \n \
                        -d inputwav \n");

                return false;
                break;
            case 'd':
                strcpy(inputFileName, optarg);
            default:
                break;
        }
    }
    return true;
}

