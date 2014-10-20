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
#include "configure_dtw.h"

class WordDtwRecognition {
    // Raw / Beam 模式
        READ_WRITE_DECLARE(WaveFeatureOP::OpType, opType, OpType);

        // 本类使用的增量threshold， 如果不用training模式的话就set
        READ_WRITE_DECLARE(double, beamThreshold, BeamThreshold);

        // 开启threshold的training模式
        READ_WRITE_DECLARE(bool, doTrainThreshold, DoTrainThreshold);

    public:
        WordDtwRecognition();
        ~WordDtwRecognition();

        // This function will load all template in a dir, 
        // if mfcc feature exist in mfccDir, it will load the feature directly 
        // Calculate also will save mfcc into mfccDir
        SP_RESULT loadTemplates(char *templateDir);


        // 异步dtw
        SP_RESULT wordAsynRecognition(std::vector<Feature> &inputFeature);

        // 同步dtw
        SP_RESULT wordSynRecognition(std::vector<Feature> &inputFeature);

        void dumpColorPath(std::ostream & Out);

        // 在做完dtw后调用返回一个模板
        const WaveFeatureOP * getBestTemplate();

        // 如果要记录路径记得先设置
        void setDoRecordPath(bool doRecord);

    private:

        void trainThreshold() {
            beamThreshold = DEFAULT_BEAM_THRESHOLD;
        }

        WaveFeatureOPSet templates;
//        std::map<std::string, std::vector<WaveFeatureOP> > templates;
};

#endif
