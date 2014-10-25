#include "resource.h"
#include "srs.h"
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <fstream>
#include "FeatureExtractor.h"
#include <unistd.h>
#include "WaveFeatureOP.h"
#include "WordDtwRecognition.h"
#include "WaveFeatureOPSet.h"
#include "ThreadPool.h"
#include <vector>
#include "tool.h"
#include "configure_dtw.h"

using namespace std;

bool isBeam;
int maxInstancePerTemplate = MAX_TEMPLATES_PER_WORD;
double threshold = DEFAULT_BEAM_THRESHOLD;

int  threadNum = DEFAULT_THREAD_NUM;
bool isCapture;
char templateFileName[1000] = "";
char inputFileName[1000] = "";

char templateDirName[256] = TEMPLATES_DIR;
char inputDirName[256] = INPUTS_DIR;

bool dealOpts(int argc, char **argv);

// -g 0(default 1-1)
// -g 1(n-1)
// -g 2(n-n)
int demoType = 0;

// ont-to-one demo
void runDemo11();

// N-to-one demo
void runDemoN1();

// N-to-N demo
void runDemoNN();

int main(int argc, char **argv) {
    if(! dealOpts(argc, argv))
        return 0;

    if(0 == demoType)
        runDemo11();

    if(1 == demoType)
        runDemoN1();

    if(2 == demoType)
        runDemoNN();

    return 0;
}
void runDemo11() {
    FeatureExtractor extractor(threadNum);

    RawData data;

    if(strlen(templateFileName) == 0) {
        Tip("[Capture a template!!]\n\n");
        capture("tmp", data, false);
    }
    else {
        Tip("[Load a template!!]\n\n");
        load_wav_file(templateFileName, data);
    }
    extractor.exDoubleDeltaFeatures(&data);

    WaveFeatureOP dtwOP(extractor.getNormalMelCepstrum());

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

    // recore Path for demo
    dtwOP.setDoRecordPath(true);
    if(isBeam)
        dtwOP.setOpType(WaveFeatureOP::Beam);
    double dist = dtwOP.asynDtw(&inputFeature, threshold);

    dtwOP.dumpColorPath(cout);

    printf("%lf\n", dist);
}
void runDemoN1() {
    FeatureExtractor extractor(threadNum);
    WordDtwRecognition recognition;

    RawData data;
    recognition.loadTemplates(templateDirName);

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

    // recore Path for demo
    recognition.setDoRecordPath(true);

    if(isBeam)
        recognition.setOpType(WaveFeatureOP::Beam);

    if(isBeam) {
        recognition.setBeamThreshold(threshold);
        recognition.wordSynRecognition(inputFeature);
    }
    else 
        recognition.wordAsynRecognition(inputFeature);

    WaveFeatureOP * finalTemp = const_cast<WaveFeatureOP *>(recognition.getBestTemplate());

    recognition.dumpColorPath(cout);

    cout << "The best template is : \n";
    finalTemp->dumpColorPath(cout);

    string fileName = finalTemp->getFileName();

    Capture::load_wav_file(fileName.c_str(), data, true);
}

void runDemoNN() {
    FeatureExtractor extractor(threadNum);
    WordDtwRecognition recognition;

    recognition.loadTemplates(templateDirName);

    if(isBeam)
        recognition.setOpType(WaveFeatureOP::Beam);

    WaveFeatureOPSet inputs;
    inputs.loadMfccs(inputDirName);

    int allCnt = 0, correctCnt = 0;

    WaveFeatureOPSet::iterator Itr = inputs.begin();
    
    for(; Itr != inputs.end(); Itr ++) {
        allCnt ++;
        recognition.setDoRecordPath(false);

        if(isBeam) {
            recognition.setBeamThreshold(threshold);
            recognition.wordSynRecognition((*Itr)->getTemplateFeature());
       }
        else 
            recognition.wordAsynRecognition((*Itr)->getTemplateFeature());

        WaveFeatureOP * finalTemp = const_cast<WaveFeatureOP *>(recognition.getBestTemplate());

        if((*finalTemp) == (*(*Itr)))
            correctCnt ++;

        cout << "Template: " << left<< setw(10) << finalTemp->getWord() << " Input: " << (*Itr)->getWord() << endl;
    }

    system("clear");
    cout << "Input " << allCnt << " wavs " << endl; 
    cout << "Correct Counts: " << correctCnt << endl; 
    /*  


    recognition.dumpColorPath(cout);

    cout << "The best template is : \n";
    finalTemp->dumpColorPath(cout);
    */
}

bool dealOpts(int argc, char **argv) {
    int c;
    while((c = getopt(argc, argv, "p:g:bB:hj:d:t:D:T:")) != -1) {
        switch(c) {
            case 'h':
                printf("usage: \n \
                        filename example: abc\n \
                        -j threadNum \n \
                        -p maxInstancePerTemplate\n \
                        -g demoType : 0(1-1) 1(n-1) 2(n-n)\n \
                        -b : 使用默认beam threshold\n \
                        -B bean_threshold : Set Beam mode \n \
                        -D input dir name \n \
                        -T template dir name\n \
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
            case 'p':
                maxInstancePerTemplate = atoi(optarg);
                WaveFeatureOPSet::maxTemplatesPerWord = maxInstancePerTemplate;
                break;
            case 'b':
                isBeam = true;
                break;

            case 'B':
                isBeam = true;
                sscanf(optarg, "%lf", &threshold);
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
            default:
                break;
        }
    }
    return true;
}
void storeFeas(const std::vector<Feature> & data, const char *filename) {
    ofstream out(filename);
    /* 
    if(data.size())
        out << data[0].size() << endl;
        */
    for(int i = 0;i < data.size(); i++) {
        int M = data[i].size();
        for(int j = 0;j < M;j++)
            out << data[i][j] << " ";
        out << endl;
    }
    out.close();
}
