//
//  FeatureExtractor.cpp
//  SpeechRecongnitionSystem
//
//  Created by Admin on 9/11/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#include "FeatureExtractor.h"
#include "RawData.h"
#include <math.h>

//void FeatureExtractor::calcFeatures(const RawData* rd){
//    preEmph(rd->getData(),rd->getFrameNum());
//    int size = (int)emp_data.size();
//    int dt = SEGMENT_SIZE-OVERLAP_SIZE;
//    for(int i = 0;i+SEGMENT_SIZE<size;i+=dt){
//        this->dealWithOneSegment((emp_data.data())+i);
//    }
//    standard();
//    normalization();
//}

SP_RESULT FeatureExtractor::exFeatures(const RawData *data) {

}

void FeatureExtractor::preEmph(const SOUND_DATA* rd,int size,double factor){
    emp_data.push_back(rd[0]);
    for(int i = 1;i<size;i++){
        emp_data.push_back(rd[i] - factor * rd[i-1]);
    }
}

//void windowing(){
    
//}

Feature FeatureExtractor::dealWithOneSegment(const double data[SEGMENT_SIZE]){
    Feature ret;
    //double tmp_data[SEGMENT_SIZE*2];
    
    int size = ceil(log2(SEGMENT_SIZE));
    size = 1<<size;
    
//    windowing(tmp_data,size);
//    paddingZero(tmp_data,size);
//    Signal2PowerSpectrum(tmp_data,size);
//    PowerSpectrum2MelSpectrum(tmp_data,size);
    return ret;
}

void FeatureExtractor::standard(){
    
}
void FeatureExtractor::normalization(){
    
}
