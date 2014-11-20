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
#include "Capture.h"

WaveFeatureOPSet::WaveFeatureOPSet(int maxInstancePer) : maxInstancePer(maxInstancePer) {}

WaveFeatureOPSet::~WaveFeatureOPSet() {}

SP_RESULT WaveFeatureOPSet::loadMfccs(char *templateDir, char *fileName, WaveFeatureOP::LOAD_TYPE loadType) {
    static FeatureExtractor extractor(ThreadPool::thread_num);
    static RawData data;
    char word[WORD_MAX_LEN];
    char user[USERNAME_MAX_LEN];
    int seqIdx;
    SerialFiles::parseSerialFileName(fileName, seqIdx, 2, user, word);


    // don't load the file, but just store its name
    // this function will be very useful if we load hmm 
    // from file, we don't need wav !!
    if(loadType == WaveFeatureOP::ONLY_FILE_NAME) {
        std::vector<Feature> features;

        WaveFeatureOP newOp(features, std::string(fileName), word);

        addWaveMfcc( word, newOp );

        return SP_SUCCESS;
    }
    // 如果有缓存，不重复计算mfcc
    if(loadMfccFromMfccFile(templateDir, fileName, word))
        return SP_SUCCESS;

    std::string tmpName = templateDir;
    tmpName += "/";
    tmpName += fileName;

    Capture::load_wav_file(tmpName.c_str(), data);

    extractor.exDoubleDeltaFeatures(&data);

    // 将结果存储到mfcc文件，节省下次计算的时间
    saveMfcc2MfccFile(templateDir, fileName, extractor.getNormalMelCepstrum());

    // OP 实际上是存储feature的类, 注意要传入名字和文件名
    // 不传也可以， 为了demo才需要正确性检测
    WaveFeatureOP dtwOP(extractor.getNormalMelCepstrum(), tmpName, word);

    addWaveMfcc(word, dtwOP);

    return SP_SUCCESS;
}

// 从目录load 所有的feature
SP_RESULT WaveFeatureOPSet::loadMfccs(char *templateDir, WaveFeatureOP::LOAD_TYPE loadType) {
    std::vector<std::string> wavFileNames;

    SerialFiles::getWavFileNames(templateDir, wavFileNames);

    for(int i = 0;i < wavFileNames.size(); i++) {
        loadMfccs(templateDir, const_cast<char *>(wavFileNames[i].c_str()), loadType);
    }
    return SP_SUCCESS;
}
