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
#include "Feature.h"
#include "RawData.h"
#include "resource.h"

class FeatureExtractor{
protected:
    std::vector<Feature> f_data;
    std::vector<double> emp_data;
    
    void inital(){
        f_data.clear();
        emp_data.clear();
    }
    
    // pre emphasize and save the data into emp_data
    // if factor == 0 then no emphasize
    void preEmph(const SOUND_DATA* rd,int size ,double factor = 0.95);
    
    //void windowing();
    // calculate the feature of data
    Feature dealWithOneSegment(const double data[SEGMENT_SIZE]);
    
    void standard();
    void normalization();
    
public:
    FeatureExtractor(){}
    ~FeatureExtractor(){}
    
//    void calcFeatures(const RawData* rd);
    
    SP_RESULT exFeatures(const RawData *data);
};
#endif /* defined(__SpeechRecongnitionSystem__FeatureExtractor__) */
