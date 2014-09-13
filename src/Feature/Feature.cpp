//
//  Feature.cpp
//  SpeechRecongnitionSystem
//
//  Created by Admin on 9/11/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#include "Feature.h"

#include <cmath>
void Feature::push_back(double d) {
    data.push_back(d);
}
Feature & Feature::meanNormalSelf() {
    double sum = 0.0;
    for(int i = 0;i < size();i++) 
        sum += (*this)[i];

    sum /= size();

    for(int i = 0;i < size(); i++) 
        (*this)[i] -= sum;

    return *this;
}
Feature & Feature::varianceNormalSelf() {
    double sdRecord = 0.0;

    for(int i = 0;i < size(); i++) sdRecord += (*this)[i] * (*this)[i];

    sdRecord /= size();

    sdRecord = sqrt(sdRecord);

    for(int i = 0;i < size(); i++) {
        (*this)[i] /= sdRecord;
    }

    return *this;
}
