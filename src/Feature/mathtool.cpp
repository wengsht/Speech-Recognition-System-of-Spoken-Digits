//
//  mathtool.cpp
//  SpeechRecongnitionSystem
//
//  Created by Admin on 9/11/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#include "mathtool.h"

#include <assert.h>

#include <iostream>
using namespace std;
const int MAXN = 1000;
// fft(a, n, 1) -- dft
// fft(a, n, -1) -- idft
// n should be 2^k
void fft(cp *a,int n,int f)
{
 //   assert(MAXN > n);
    cp *b = new cp[n];
    double arg = PI;
    for(int k = n>>1;k;k>>=1,arg*=0.5){
        cp  wm = std::polar(f*arg),w(1,0);
        for(int i = 0;i<n;i+=k,w*=wm){
            int p = i << 1;
            if(p>=n) p-= n;
            for(int j = 0;j<k;++j){
                b[i+j] = a[p+j] + w*a[p+k+j];
            }
        }
        for(int i = 0;i<n;++i) a[i] = b[i];
    }
    delete []b;
}

// use to check fft is right
void dft(cp *a,int n,int f)
{
    cp *b = new cp[n];
    for(int i = 0;i < n;i++) {
        b[i] = cp(0, 0);

        for(int j = 0;j < n;j++) {

            b[i] += cp(std::real(a[j])*cos(-2.0*PI*j*i/n), std::real(a[j])*sin(-2.0*PI*j*i/n));
        }
    }
    for(int i = 0;i<n;++i) a[i] = b[i];

    delete []b;
}


void dct(double *a,int n,int f)
{
    
}
