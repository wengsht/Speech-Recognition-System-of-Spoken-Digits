//
//  FeatureExtractor.h
//  SpeechRecongnitionSystem
//
//  Created by Admin on 9/11/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef __SpeechRecongnitionSystem__FeatureExtractor__
#define __SpeechRecongnitionSystem__FeatureExtractor__

#include <iostream>
#include <vector>
#include <cmath>
#include "mathtool.h"
#include "Feature.h"
#include "RawData.h"
#include "resource.h"
#include <unistd.h>
#include <pthread.h>

class FeatureExtractor{
    CONST_REFERENCE_READ_ONLY_DECLARE(std::vector<double> , emp_data, EmpData)
    CONST_REFERENCE_READ_ONLY_DECLARE(Matrix<double> , windows, Windows)
    CONST_REFERENCE_READ_ONLY_DECLARE(Matrix<double> , powSpec, PowSpectrum)
    CONST_REFERENCE_READ_ONLY_DECLARE(Matrix<double> , melLogSpec, MelLogSpec)

    CONST_REFERENCE_READ_ONLY_DECLARE(std::vector<Feature>, melCeps, MelCepstrum);
    CONST_REFERENCE_READ_ONLY_DECLARE(std::vector<Feature>, normalMelCeps, NormalMelCepstrum);
private:
    static void paddingTask(void *in);
    struct padding_task_info {
        std::vector<double> * window;
        int nfft;
    };
    static void fftTask(void *in);
    struct fft_task_info {
        std::vector<double> * window;
        std::vector<double> * powWinSpec;
    };
    static void mulTask(void *in);
    struct mul_task_info {
        std::vector<double> * wts;
        Matrix<double> * powSpec;
        std::vector<double> *melLog;
    };
protected:

//    std::vector<Feature> melCeps;
    void inital(){
        melCeps.clear();
        emp_data.clear();
        melLogSpec.clear();
        powSpec.clear();
        windows.clear();
    }
    
    // pre emphasize and save the data into emp_data
    // if factor == 0 then no emphasize
    SP_RESULT preEmph(std::vector<double> &outs, \
            const SOUND_DATA* rd, \
            int size , \
            double factor = SP_PREEMPH_FACTOR);

    SP_RESULT windowing(Matrix<double> & out_windows, \
            const std::vector<double> &in, \
            double winTime = WINTIME, \
            double stepTime = STEPTIME, \
            int rate = SAMPLE_RATE, \
            double (*winFunc)(int, int) = FeatureExtractor::hanning);

    SP_RESULT fftPadding(Matrix<double> & out_pads);
    
    SP_RESULT powSpectrum(Matrix<double> &powSpectrum, Matrix<double> &windows);

    SP_RESULT melCepstrum(std::vector<Feature> &cepstrums, \
            const Matrix<double> &melLogSpec, \
            int cepsNum = CEPS_NUM);

    static std::vector<double> & windowFFT(std::vector<double> &res, \
            std::vector<double> &data);


    static double hanning(int n, int M) {
        return 0.5 - 0.5 * cos (2.0 * PI * n / M);
    }
    static double hz2mel(double frequency) {
        return 2595.0 * log(1+frequency/700.0) / log(10.0);
    }
    static double mel2hz(double hz) {
        return 700.0* ( pow(10.0, hz/2595.0) - 1.0);
    }
    static double getDB(double pow) {
        return 10.0 * log(pow) / log(10.0);
    }
    /* 
     * melLog = wts * powSpec'
     * */
    SP_RESULT MatrixMul01(Matrix<double> & melLog, \
            Matrix<double> &wts, \
            Matrix<double> & powSpec);

    SP_RESULT fft2MelLog(int nfft, \
            Matrix<double> &melLog, \
            Matrix<double> & powSpec, \
            int nfilts = MEL_FILTER_NUM, \
            double (*hz2melFunc)(double) = FeatureExtractor::hz2mel, \
            double (*mel2hzFunc)(double) = FeatureExtractor::mel2hz, \
            double minF = MIN_F, \
            double maxF = MAX_F, \
            int sampleRate = SAMPLE_RATE);

    SP_RESULT mel2dct(Feature & feature, std::vector<double> melLog, int cepsNum = CEPS_NUM);

    SP_RESULT normalization(std::vector<Feature> &normalMels,const std::vector<Feature> & melFes);

//    SP_RESULT getMelLog(std::vector<double> & melLog, \
            const std::vector<double> & powSpec, \
            const Matrix<double> &wts);

    SP_RESULT getWts(Matrix<double> &wts, \
            int nfft, \
            double minF = MIN_F, \
            double maxF = MAX_F, \
            int sampleRate = SAMPLE_RATE, \
            int nfilts = MEL_FILTER_NUM, \
            double (*hz2melFunc)(double) = FeatureExtractor::hz2mel, \
            double (*mel2hzFunc)(double) = FeatureExtractor::mel2hz);

    SP_RESULT windowMul(std::vector<double> &window, \
            double (*winFunc)(int, int) );
    
public:
    FeatureExtractor() :threadNum(DEFAULT_THREAD_NUM) {}
    FeatureExtractor(int threadNum) : threadNum(threadNum) {}
    ~FeatureExtractor() {}
    
//    void calcFeatures(const RawData* rd);
    
    SP_RESULT exFeatures(const RawData *data, \
            int sampleRate = SAMPLE_RATE, \
            double preEmpFactor = SP_PREEMPH_FACTOR, \
            double winTime = WINTIME, \
            double stepTime = STEPTIME, \
            double (*winFunc)(int, int) = FeatureExtractor::hanning, \
            double minF = MIN_F, \
            double maxF = MAX_F, \
            double (*hz2melFunc)(double) = FeatureExtractor::hz2mel, \
            double (*mel2hzFunc)(double) = FeatureExtractor::mel2hz, \
            int nfilts = MEL_FILTER_NUM, \
            int cepsNum = CEPS_NUM);

private:
    int threadNum;
};
#endif /* defined(__SpeechRecongnitionSystem__FeatureExtractor__) */
