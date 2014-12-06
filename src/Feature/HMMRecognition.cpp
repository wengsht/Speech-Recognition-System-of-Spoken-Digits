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
#include "WaveFeatureOP.h"
#include "WaveFeatureOPSet.h"


HMMRecognition::HMMRecognition(int stateNum, int gaussNum, int trainTimes) : stateNum(stateNum), gaussNum(gaussNum), trainTimes(trainTimes), automatons(stateNum, gaussNum, trainTimes) {
}
HMMRecognition::~HMMRecognition() {
}
SP_RESULT HMMRecognition::loadTemplates(char *templateDir, WaveFeatureOP::LOAD_TYPE loadType) {
    automatons.loadMfccs(templateDir, loadType);

    this->templateDir = string(templateDir);

    return SP_SUCCESS;
}

string HMMRecognition::generateHMMFileName(char *suffix) {
    static char buf[1024];
    static std::string algo;

    if(stateType == HMMState::KMEAN)
        algo = "kmean";
    else 
        algo = "soft";

    sprintf(buf, "%s/%s_%d_%d_%d.hmm%s",  templateDir.c_str(),algo.c_str(), stateNum, gaussNum, stateNum, suffix);

    return string(buf);
//    res += string(stateNum);
//
}

bool HMMRecognition::loadHMMModel(HMMAutomatonSet & automatons, char *suffix) {
    string fileName = generateHMMFileName(suffix);

    ifstream in;

    in.open(fileName, std::ios::in);
    if(in.fail() == true)
        return false;
    
    automatons.load(in);

    in.close();

//    return false;
    return true;
}

void HMMRecognition::storeHMMModel() {
    string fileName = generateHMMFileName();
    ofstream out(fileName);

    automatons.store(out);

    out.close();
}
 

SP_RESULT HMMRecognition::hmmTryLoad(char *templateDir) {
    this->templateDir = string(templateDir);

    automatons.setStateType(stateType);

    if(loadHMMModel(automatons)) {
//        loadTemplates(templateDir, WaveFeatureOP::ONLY_FILE_NAME);
        return SP_SUCCESS;
    }

    return SP_LOAD_FAIL;
}

SP_RESULT HMMRecognition::hmmTrain(HMMAutomaton::TRAIN_TYPE type) {
    automatons.setStateType(stateType);

    // load the model from buffer
    if(loadHMMModel(automatons)) ;
    else {
        loadHMMModel(automatons, INIT_MODEL_SUFFIX);

        automatons.train(type);

        storeHMMModel();
    }

    return SP_SUCCESS;
}

void HMMRecognition::dumpAutomaton(std::ostream & out) {
    automatons.dumpAutomaton(out);
}

std::string HMMRecognition::hmmRecognition(WaveFeatureOP & input) {
    return automatons.recognition(input);
}

void HMMRecognition::close() {
    automatons.clear();
}
