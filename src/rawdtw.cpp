/*
 * 
 *
 *   This is used to justify the dtw homework part.1
 *
 *   use: ./rawdtw -t haha_cut -d one_00000004
 *
 *   will output a dtw value
 *
 */
#include "resource.h"
#include "srs.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "FeatureExtractor.h"
#include <unistd.h>
#include "WaveFeatureOP.h"
#include <vector>
#include "tool.h"

using namespace std;

bool isBeam;
double threshold;

int  threadNum = DEFAULT_THREAD_NUM;

char templateFileName[1000] = "";
char inputFileName[1000] = "";

bool dealOpts(int argc, char **argv);

// ont-to-one demo
void runDemo11();
// 显示路径

int main(int argc, char **argv) {
    if(! dealOpts(argc, argv))
        return 0;

    runDemo11();

    return 0;
}

void rawDtw(vector<Feature> a, vector<Feature> b) {
    const int siz = 1000;
    double dtw[siz][siz];
    if(a.size() > siz || b.size() > siz) 
        return ;
    int asiz = a.size(), bsiz = b.size();

    for(int i = 0;i < asiz;i++) 
        for(int j = 0;j < bsiz;j++) 
            dtw[i][j] = -1e18;

    dtw[0][0] = a[0] - b[0];
    dtw[1][0] = a[1] - b[0];
    dtw[2][0] = a[2] - b[0];
    for(int j = 1;j < bsiz;j++) {
        for(int i = 0;i < asiz;i++) {
            double add = a[i] - b[j];
                dtw[i][j] = add + dtw[i][j-1];

            if(i - 1 >= 0) 
                if(dtw[i][j] < add+dtw[i-1][j-1])
                    dtw[i][j] = add + dtw[i-1][j-1];
            if(i - 2 >= 0) 
                if(dtw[i][j] < add+dtw[i-2][j-1])
                    dtw[i][j] = add + dtw[i-2][j-1];
        }
    }

    double res = -1e18;
    for(int i = 0;i < asiz; i++) {
        if(dtw[i][bsiz-1] != 1.0 && res < dtw[i][bsiz-1])
            res = dtw[i][bsiz-1];
    }
    printf("%lf\n", res);
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

    vector<Feature> templateFeature = extractor.getNormalMelCepstrum();
//    WaveFeatureOP dtwOP(extractor.getNormalMelCepstrum());

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

    rawDtw(templateFeature, inputFeature);

    // recore Path for demo
//    dtwOP.setDoRecordPath(true);
//    if(isBeam)
//        dtwOP.setOpType(WaveFeatureOP::Beam);
//    double dist = dtwOP.asynDtw(&inputFeature, threshold);

//    const vector<int> & path = dtwOP.getPath();

//    showPath(path, dtwOP.getRowNum());

//    printf("%lf\n", dist);
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
