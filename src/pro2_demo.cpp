#include "resource.h"
#include "srs.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include "FeatureExtractor.h"

using namespace std;

void reportMatlab(FeatureExtractor &extractor);
int main() {
    FeatureExtractor extractor;

    RawData data;

    load_wav_file("output", data);

    extractor.exFeatures(&data);

    reportMatlab(extractor);

    return 0;
}
template <class T> 
void storeVector(const vector<T> data, const char *filename) {
    ofstream out(filename);
    for(int i = 0;i < data.size(); i++) 
        out << data[i] << endl;
    out.close();
}
template <class T> 
void storeMatrix(const Matrix<T> data, const char *filename) {
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
void reportMatlab(FeatureExtractor &extractor) {
    const vector<double> &empData = extractor.getEmpData();

    storeVector(empData, "emp.txt");

    const Matrix<double> &powSpec = extractor.getPowSpectrum();

    storeMatrix(powSpec, "powSpec.txt");

    const Matrix<double> &melLog = extractor.getMelLogCeps();

    storeMatrix(melLog, "melLogCeps.txt");
}
