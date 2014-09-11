//
//  Feature.h
//  SpeechRecongnitionSystem
//
//  Created by Admin on 9/11/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef __SpeechRecongnitionSystem__Feature__
#define __SpeechRecongnitionSystem__Feature__

#include <vector>
#include <iostream>
// feature is a vector
class Feature{
protected:
    std::vector<double> data;
public:
    Feature operator * (const Feature & T)const{
        Feature ret;
        
        return ret;
    }
    
    Feature sqr(){
        Feature ret;
        return ret;
    }
    
    Feature operator + (const Feature & T)const{
        Feature ret;
        
        return ret;
    }
    
    Feature operator * (double c)const{
        Feature ret;
        
        return ret;
    }
};
#endif /* defined(__SpeechRecongnitionSystem__Feature__) */
