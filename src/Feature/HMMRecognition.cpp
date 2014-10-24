// =====================================================================================
// 
//       Filename:  HMMRecognition.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/21 21时41分09秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
//

#include "HMMRecognition.h"


HMMRecognition::HMMRecognition(int stateNum, int gaussNum, int trainTimes) : stateNum(stateNum), gaussNum(gaussNum), trainTimes(trainTimes), automatons(stateNum, gaussNum, trainTimes) {
}
HMMRecognition::~HMMRecognition() {
}
SP_RESULT HMMRecognition::loadTemplates(char *templateDir) {
    automatons.loadMfccs(templateDir);

    return SP_SUCCESS;
}

SP_RESULT HMMRecognition::hmmTrain() {
    automatons.setStateType(stateType);

    automatons.train();

    return SP_SUCCESS;
}

std::string HMMRecognition::hmmRecognition(WaveFeatureOP & input) {
    return automatons.recognition(input);
}

void HMMRecognition::close() {
    automatons.clear();
}
