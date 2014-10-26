// =====================================================================================
// 
//       Filename:  WordDtwRecognition.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/19 23时09分11秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "WordDtwRecognition.h"
#include "Feature.h"

WordDtwRecognition::WordDtwRecognition() : doTrainThreshold(false) {}

WordDtwRecognition::~WordDtwRecognition() {}

SP_RESULT WordDtwRecognition::loadTemplates(char *templateDir) {
    return templates.loadMfccs(templateDir);
}

// 开启路径记录， 会把每一个底层的template都开启
void WordDtwRecognition::setDoRecordPath(bool doRecord) {
    WaveFeatureOPSet::iterator Itr = templates.begin();
    
    for(; Itr != templates.end(); Itr ++) {
        (*Itr)->setDoRecordPath(doRecord);
    }
}

// 对整个模板库异步
SP_RESULT WordDtwRecognition::wordAsynRecognition(std::vector<Feature> &inputFeature) {
    if(doTrainThreshold && opType == WaveFeatureOP::Beam)
        trainThreshold();

    WaveFeatureOPSet::iterator Itr = templates.begin();
    
    for(; Itr != templates.end(); Itr ++) {
        (*Itr)->setOpType(opType);

        (*Itr)->asynDtw(&inputFeature, beamThreshold);
    }
    return SP_SUCCESS;
}

// 对整个模板库同步
SP_RESULT WordDtwRecognition::wordSynRecognition(std::vector<Feature> &inputFeature) {
    if(doTrainThreshold && opType == WaveFeatureOP::Beam)
        trainThreshold();

    // init every template
    WaveFeatureOPSet::iterator Itr = templates.begin();
    
    for(; Itr != templates.end(); Itr ++) {
        (*Itr)->setOpType(opType);

        (*Itr)->synInit(&inputFeature);
    }

    double bestValue = 0.0; // Feature::IllegalDist;
    // asyn forwarding
    for(int Cnt = 0; Cnt < inputFeature.size(); Cnt ++) {
        double partBestValue = Feature::IllegalDist;
        for(Itr = templates.begin(); Itr != templates.end(); Itr ++) {
            (*Itr)->setOpType(opType);

            double tmpRes = (*Itr)->forwardColumn(bestValue + beamThreshold);

            if(Feature::better(tmpRes, partBestValue)) 
                partBestValue = tmpRes;
        }

        bestValue = partBestValue;
    }

    return SP_SUCCESS;
}

// 返回结果
const WaveFeatureOP * WordDtwRecognition::getBestTemplate() {
    double bestValue = Feature::IllegalDist;

    WaveFeatureOP * resTemplate = NULL;

    WaveFeatureOPSet::iterator Itr = templates.begin();
    
    for(; Itr != templates.end(); Itr ++) {
        if(Feature::better((*Itr)->getDtwResult(), bestValue)) {
            resTemplate = *Itr;
            bestValue = (*Itr)->getDtwResult();
        }
    }
    return resTemplate;
}

void WordDtwRecognition::dumpColorPath(std::ostream &Out) {
    WaveFeatureOPSet::iterator Itr = templates.begin();
    
    for(; Itr != templates.end(); Itr ++) {
        (*Itr)->dumpColorPath(Out);
    }
}

void WordDtwRecognition::setMaxInstancePerWord(int maxInstancePer) {
    templates.setMaxInstancePerWord(maxInstancePer);
}
