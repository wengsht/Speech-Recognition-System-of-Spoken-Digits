//
//  mathtool.h
//  SpeechRecongnitionSystem
//
//  Created by Admin on 9/11/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef __SpeechRecongnitionSystem__mathtool__
#define __SpeechRecongnitionSystem__mathtool__

#include <iostream>
#include <complex>

const double PI = std::acos(-1);

typedef std::complex<double> cp;

void dft(cp *a,int n,int f);
void fft(cp *a,int n,int f);
//size(a) > 2*n
void dct(double *a,int n,int f);

#endif /* defined(__SpeechRecongnitionSystem__mathtool__) */
