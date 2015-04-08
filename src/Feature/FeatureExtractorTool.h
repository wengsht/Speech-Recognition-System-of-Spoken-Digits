#pragma once
#ifndef __SpeechRecognitionSystem__FeatureExtractionTool__
#define __SpeechRecognitionSystem__FeatureExtractionTool__

#include <iostream>
#include <complex>
#include "mathtool.h"

//#define e 2.718281828459
//typedef std::complex<double> cp;
//#ifndef __SpeechRecongnitionSystem__PI__
//#define __SpeechRecongnitionSystem__PI__
//const double PI = std::acos(-1);
//#endif

__global__
void windowFFT_cu(cp *d_SpeechSignal, int frameNum, int frameSize, int f, double arg=PI);

__global__ 
void fft_cu_part(cp *d_SpeechSignal, int n, int f, double arg=PI);

__device__ 
void mulComplex(cp *output, cp *input1, cp *input2);

__device__ 
void addComplex(cp *output, cp *input1, cp *input2);

__device__
void getRealImag(double& real, double& imag, const cp *input);

__device__
void getPolarValue(double length, double angle, double* output);

#endif
