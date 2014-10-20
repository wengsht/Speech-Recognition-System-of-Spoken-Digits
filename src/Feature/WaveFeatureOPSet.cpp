// =====================================================================================
// 
//       Filename:  WaveFeatureOPSet.cpp
// 
//    Description:  
//
// 
//        Version:  0.01
//        Created:  2014/10/20 07时43分56秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "WaveFeatureOPSet.h"
#include <vector>
#include <string>
#include <iostream>
#include "SerialFiles.h"
#include "FeatureExtractor.h"
#include "configure_dtw.h"
#include "ThreadPool.h"
#include "test.h"
#include "Capture.h"

int WaveFeatureOPSet::maxTemplatesPerWord = MAX_TEMPLATES_PER_WORD;

WaveFeatureOPSet::WaveFeatureOPSet() {}

WaveFeatureOPSet::~WaveFeatureOPSet() {}

SP_RESULT WaveFeatureOPSet::loadMfccs(char *templateDir, char *fileName) {
    static FeatureExtractor extractor(ThreadPool::thread_num);
    static RawData data;
    char word[WORD_MAX_LEN];
    char user[USERNAME_MAX_LEN];
    int seqIdx;
    SerialFiles::parseSerialFileName(fileName, seqIdx, 2, word, user);

    // 如果有缓存，不重复计算mfcc
    if(loadMfccFromMfccFile(templateDir, fileName, word))
        return SP_SUCCESS;

    std::string tmpName = templateDir;
    tmpName += "/";
    tmpName += fileName;

    Capture::load_wav_file(tmpName.c_str(), data);

    extractor.exDoubleDeltaFeatures(&data);

    saveMfcc2MfccFile(templateDir, fileName, extractor.getNormalMelCepstrum());

    WaveFeatureOP dtwOP(extractor.getNormalMelCepstrum(), tmpName, word);

    addWaveMfcc(word, dtwOP);

    return SP_SUCCESS;
}
SP_RESULT WaveFeatureOPSet::loadMfccs(char *templateDir) {
    std::vector<std::string> wavFileNames;

    SerialFiles::getWavFileNames(templateDir, wavFileNames);

    for(int i = 0;i < wavFileNames.size(); i++) {
        loadMfccs(templateDir, const_cast<char *>(wavFileNames[i].c_str()));
    }
    return SP_SUCCESS;
}
