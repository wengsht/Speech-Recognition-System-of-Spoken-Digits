
#include "FeatureExtractorTool.h"

__global__
void windowFFT_cu(cp *d_SpeechSignal, int frameNum, int frameSize, int f, int selIdx, double arg){
    extern __shared__ char s_SpeechSignal[];
    int p, i, j, rollIdx=0, oldRollIdx;
    size_t innerIdx = threadIdx.x % frameSize, 
           frame_offset = blockDim.x*blockIdx.x+(threadIdx.x/frameSize)*frameSize;
    double temp_cp[2], temp_wm[2], temp_w[2];
    cp *temp = (cp *) temp_cp, 
       *wm = (cp*)temp_wm, 
       *w = (cp*)temp_w; 
    //cp *d_signal[2];
    cp *s_signal[2]; 
    
    size_t sharedSize = blockDim.x * sizeof(cp);
    s_signal[0] = (cp *)s_SpeechSignal;
    s_signal[1] = (cp *)&s_SpeechSignal[sharedSize];
    //d_signal[0] = d_SpeechSignal+frame_offset;
    //d_signal[1] = d_signal[0]+frameNum*frameSize;
    
    *(s_signal[0]+innerIdx) = *(d_SpeechSignal+frame_offset+innerIdx);
    __syncthreads();
    
    for(int k = frameSize>>1; k; k>>=1, arg*=0.5){
        rollIdx ^= 1;
        oldRollIdx = rollIdx^1;
        
        getPolarValue(1, f*arg, temp_wm);
        *temp_w = 1;
        *(temp_w+1) = 0;
        
        i = innerIdx/k;
        j = innerIdx%k;
        for(int t=0; t<i; t++){
            //w = w*wm;
            mulComplex(w,wm,w);
        }
        i = i*k;
        p = i<<1;
        if(p>=frameSize) p-=frameSize;
    
        //mulComplex(temp, w, d_signal[oldRollIdx]+(p+k+j)); 
        //addComplex(d_signal[rollIdx]+(i+j), temp, d_signal[oldRollIdx]+(p+j));
        
        mulComplex(temp, w, s_signal[oldRollIdx]+(p+k+j)); 
        addComplex(s_signal[rollIdx]+(i+j), temp, s_signal[oldRollIdx]+(p+j));
        __syncthreads();
    }
    d_SpeechSignal[frame_offset+innerIdx] = *(s_signal[selIdx]+innerIdx);
}

__global__ 
void fft_cu_part(cp *d_SpeechSignal, int n, int f, double arg){
    int p, i, j, idx, rollIdx=0, oldRollIdx;
    cp* d_signal[2]; 
    d_signal[0] = d_SpeechSignal;
    d_signal[1] = &d_SpeechSignal[n];
    
    int *finalRollIdx = (int *) &d_SpeechSignal[2*n];
    
    idx = blockDim.x*blockIdx.x + threadIdx.x;
    
    //double arg = pi;
    double temp_cp[2], temp_wm[2], temp_w[2];
    cp *temp = (cp *) temp_cp, *wm = (cp*)temp_wm, *w = (cp*)temp_w;
    for(int k = n>>1; k; k>>=1, arg*=0.5){
        rollIdx ^= 1;
        oldRollIdx = rollIdx^1;
        
        //cp wm = std::polar(1.0,f*arg), w(1,0);
        getPolarValue(1, f*arg, temp_wm);
        *temp_w = 1;
        *(temp_w+1) = 0;
        
        i = idx/k;
        j = idx%k;
        for(int t=0; t<i; t++){
            //w = w*wm;
            mulComplex(w,wm,w);
        }
        i = i*k;
        p = i<<1;
        if(p>=n) p-=n;
    
        //d_signal[rollIdx][i+j] = d_signal[oldRollIdx][p+j] + w*d_signal[oldRollIdx][p+k+j];
        mulComplex(temp, w, &d_signal[oldRollIdx][p+k+j]); 
        addComplex(&d_signal[rollIdx][i+j], temp, &d_signal[oldRollIdx][p+j]);
        __syncthreads();
    }
    if(idx==0)
        *finalRollIdx = rollIdx;
}

__device__ 
void mulComplex(cp *output, cp *input1, cp *input2){
    double real1, imag1, real2, imag2;
    getRealImag(real1,imag1,input1);
    getRealImag(real2,imag2,input2);
    double *ptr_output = (double *)output;
    *ptr_output = real1*real2-imag1*imag2;
    *(ptr_output+1) = real1*imag2+imag1*real2;
    //output = cp( real1*real2-imag1*imag2 , real1*imag2+imag1*real2 );
}

__device__
void addComplex(cp *output, cp *input1, cp *input2){
    double real1, imag1, real2, imag2;
    getRealImag(real1,imag1,input1);
    getRealImag(real2,imag2,input2);
    double *ptr_output = (double *)output;
    *ptr_output = real1+real2;
    *(ptr_output+1) = imag1+imag2;
    //output = cp( real1+real2, imag1+imag2 );
}

__device__
void getRealImag(double& real, double& imag, const cp *input){
    double *comp = (double *)input;
    real = *comp;
    imag = *(comp+1);
}

__device__
void getPolarValue(double rho, double theta, double *output){
    *output = rho*cos(theta);
    *(output+1) = rho*sin(theta);
}

