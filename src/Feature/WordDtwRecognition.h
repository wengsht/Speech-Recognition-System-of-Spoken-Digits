// =====================================================================================
// 
//       Filename:  WordDtwRecognition.h
// 
//    Description:  This class load templates and do recognition on word
//                  Using simple MFCC Dtw algorithm
// 
//        Version:  0.01
//        Created:  2014/10/19 22时35分15秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_WordDtwRecognition_H_
#define _AUTOGUARD_WordDtwRecognition_H_

#include "WaveFeatureOP.h"
#include <string>
#include "WaveFeatureOPSet.h"
#include <map>
#include <vector>
#include <iostream>

class WordDtwRecognition {
        READ_WRITE_DECLARE(WaveFeatureOP::OpType, opType, OpType);
        READ_WRITE_DECLARE(double, beamThreshold, BeamThreshold);

    public:
        WordDtwRecognition();
        ~WordDtwRecognition();

        // This function will load all template in a dir, 
        // if mfcc feature exist in mfccDir, it will load the feature directly 
        // Calculate also will save mfcc into mfccDir
        SP_RESULT loadTemplates(char *templateDir);

        SP_RESULT wordAsynRecognition(std::vector<Feature> &inputFeature);

        SP_RESULT wordSynRecognition(std::vector<Feature> &inputFeature);

        void dumpColorPath(std::ostream & Out);

        const WaveFeatureOP * getBestTemplate();

        void setDoRecordPath(bool doRecord);

    private:

        void trainThreshold() {
            beamThreshold = -10.0;
        }

        WaveFeatureOPSet templates;
//        std::map<std::string, std::vector<WaveFeatureOP> > templates;
};

#endif
