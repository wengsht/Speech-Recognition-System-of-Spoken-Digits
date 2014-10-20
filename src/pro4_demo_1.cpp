#include "resource.h"
#include "srs.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "FeatureExtractor.h"
#include <unistd.h>
#include "WaveFeatureOP.h"
#include "WordDtwRecognition.h"
#include <vector>
#include "tool.h"
#include "configure_dtw.h"

using namespace std;

bool isBeam;
double threshold;

int  threadNum = DEFAULT_THREAD_NUM;
bool isCapture;
char templateFileName[1000] = "";
char inputFileName[1000] = "";

char dirName[256] = TEMPLATES_DIR;

bool dealOpts(int argc, char **argv);

// ont-to-one demo
void runDemo11();

// N-to-one demo
void runDemoN1();

int main(int argc, char **argv) {
    if(! dealOpts(argc, argv))
        return 0;

//    runDemo11();

    runDemoN1();

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
    recognition.loadTemplates(dirName);

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

    if(isBeam)
        recognition.wordSynRecognition(inputFeature);
    else 
        recognition.wordAsynRecognition(inputFeature);

    recognition.dumpColorPath(cout);
}

bool dealOpts(int argc, char **argv) {
    int c;
    while((c = getopt(argc, argv, "B:hj:d:t:")) != -1) {
        switch(c) {
            case 'h':
                printf("usage: \n \
                        filename example: abc\n \
                        -j threadNum \n \
                        -B bean_threshold : Set Beam mode \n \
                        -d input file name[capture if not set] \n \
                        -t template file name[capture if not set]\n");

                return false;
                break;
            case 'j':
                threadNum = atoi(optarg);
                break;
            case 'B':
                isBeam = true;
                sscanf(optarg, "%lf", &threshold);
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