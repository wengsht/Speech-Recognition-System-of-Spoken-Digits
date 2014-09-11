//
//  Capture.h
//  SpeechRecongnitionSystem
//
//  Created by Admin on 8/26/14.
//  Copyright (c) 2014 Admin. All rights reserved.
//

#ifndef __SpeechRecongnitionSystem__Capture__
#define __SpeechRecongnitionSystem__Capture__

#include "RawData.h"
#include "portaudio.h"
//do the initial job ,etc. 
class Capture{
protected:
    //basic
    PaStreamParameters in;
    PaStreamParameters out;
    PaStream * stream;
    

    // initial and end
    bool init_stream(bool input );
    bool init_PA();
    
    // the basic class don't use callback
    bool init(RawData * data,bool input );
    bool end();
    
    //use in callback model;
    PaStreamCallback* callback;
    void * userData;
    
    virtual bool captureAction(RawData * data) ;
    virtual bool playAction(RawData * data) ;
    virtual bool init_callback(RawData * data,bool input);

public:
    Capture();
    ~Capture();
    
    // 
    bool  capture(RawData *);
    bool  play(RawData *);
    
};




#endif /* defined(__SpeechRecongnitionSystem__Capture__) */
