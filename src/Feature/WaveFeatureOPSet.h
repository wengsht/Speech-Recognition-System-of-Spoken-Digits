// =====================================================================================
// 
//       Filename:  WaveFeatureOPSet.h
// 
//    Description:  This class store a set of WaveFeatureOP
//                  iterator ++ : 遍历所有单词
//                  nextWord()  : 跳到下一个单词的库
// 
//        Version:  0.01
//        Created:  2014/10/20 07时43分14秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_WaveFeatureOPSet_H_
#define _AUTOGUARD_WaveFeatureOPSet_H_

#include <set>
#include <string>
#include <map>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Feature.h"

#include "WaveFeatureOP.h"
#include "SerialFiles.h"
#include "configure_dtw.h"
#include "configure_feature.h"

class WaveFeatureOPSet {
    READ_WRITE_DECLARE(int, maxInstancePer, MaxInstancePerWord);
    public:
        // 控制变量， 每一个template用多少个实例做dtw
        // 反应在了iterator的++上，当一个iterator在同一个模板的实例上迭代次数超过了这个值，就会跳到下一个template

        // 底层存储是一个从 word到vector的map， 一个word有多个template实例
        typedef std::map< std::string, std::vector<WaveFeatureOP> > dataSetType;

        // 用于遍历模板库，会先遍历一个word的所有instance，然后接下一个word
        class iterator {
            private:
                int maxTemplatesPerWord;
            public:
                iterator(int maxInstancePer = MAX_TEMPLATES_PER_WORD) : dataSet(NULL), vecIdx(-1), maxTemplatesPerWord(maxInstancePer) { }
                iterator(dataSetType *dataSetPtr, dataSetType::iterator Itr, int idx, int maxInstancePer = MAX_TEMPLATES_PER_WORD) : dataSet(dataSetPtr), I(Itr), vecIdx(idx), maxTemplatesPerWord(maxInstancePer) {}

                WaveFeatureOP * operator *() {
                    std::vector<WaveFeatureOP> & tmpVec = I->second;

                    if(vecIdx >= 0 && vecIdx < tmpVec.size()) {
                        return &tmpVec[vecIdx];
                    }

                    return NULL;
                }
                const iterator & operator ++(int) {
                    ++ (*this);

                    return *this;
                }
                const iterator & operator ++() {
                    std::vector<WaveFeatureOP> & tmpVec = I->second;

                    vecIdx ++;
                    if(vecIdx >= maxTemplatesPerWord || vecIdx >= tmpVec.size()) {
                        nextWord();
                    }
                    if(I == dataSet->end())
                        vecIdx = -1;

                    return *this;
                }

                // 跳到下一个单词的templates库
                const iterator & nextWord() {
                    I ++;
                    vecIdx = 0;
                    if(I == dataSet->end())
                        vecIdx = -1;
                    return *this;
                }
                bool operator ==(const iterator & b) {
                    return vecIdx == b.vecIdx && I == b.I;
                }
                bool operator !=(const iterator & b) {
                    return !((*this) == (b));
                }

            private:
                dataSetType *dataSet;

                dataSetType::iterator I;

                int vecIdx;
        };
        // TODO better design for begin() and end() ?
        iterator begin() {
            _begin = iterator(&dataSet, dataSet.begin(), 0, maxInstancePer);
            return _begin;
        }
        iterator end() {
            _end = iterator(&dataSet, dataSet.end(), -1);

            return _end;
        }

        // add features from a directory 
        virtual SP_RESULT loadMfccs(char *templateDir, WaveFeatureOP::LOAD_TYPE loadType = WaveFeatureOP::FULL_LOAD);
        // add features from a file
        virtual SP_RESULT loadMfccs(char *templateDir, char *fileName, WaveFeatureOP::LOAD_TYPE loadType = WaveFeatureOP::FULL_LOAD);
        WaveFeatureOPSet(int maxInstancePer = MAX_TEMPLATES_PER_WORD);
        virtual ~WaveFeatureOPSet();

private:

    iterator _begin, _end;

    // 对于1.wav 尝试从1.mfcc 中load mfcc
    //
    bool loadMfccFromMfccFile(char * dir, char * wavFileName, char * word) {
        std::string mfccFileName;

        mfccFileName = SerialFiles::getMfccFileName(wavFileName);

        std::string tmpName = dir;
        tmpName += "/";
        tmpName += mfccFileName;

        std::ifstream FIN;
        
        FIN.open(tmpName, std::ios::in);

        if(FIN.fail() == true) {
            return false;
        }

        std::string tmp;
        std::vector<Feature> features;
        while(getline(FIN, tmp)) {
            std::stringstream sin(tmp);

            features.push_back(Feature());
            int idx = features.size() - 1;
            double value;

            while(sin >> value)
                features[idx].push_back(value);
        }

        tmpName = dir;
        tmpName += "/";
        tmpName += wavFileName;

        WaveFeatureOP newOp(features, tmpName, word);
        addWaveMfcc(word, newOp);

        FIN.close();

        return true;
    }
    // 如果有计算mfcc的过程，会把一个文件的mfcc buffer到对应的mfcc文件( 1.wav -> 1.mfcc)
    // 下次再load这个文件的时候就直接读mfcc文件了
    void saveMfcc2MfccFile(char * dir, char * wavFileName, const std::vector<Feature> & features) {
        std::string mfccFileName;

        mfccFileName = SerialFiles::getMfccFileName(wavFileName);
        std::string tmpName = dir;
        tmpName += "/";
        tmpName += mfccFileName;

        std::ofstream FOUT(tmpName);

        std::string tmp;

        for(int i = 0;i < features.size(); i++) {
            std::stringstream sout("");

            sout.precision(DATA_PRECISION);
            if(features[i].size() == 0) continue;
            sout << features[i][0];

            for(int j = 1;j < features[i].size();j++) {
                sout << " ";
                sout << features[i][j];
            }
            
            FOUT << sout.str() << std::endl;
        }
        FOUT.close();
    }
private:
    // 对于map<string, vector> 的统一处理
    void addWaveMfcc(char *word, WaveFeatureOP & newOP) {
        std::string wordStr = std::string(word);

        if(!dataSet.count(wordStr)) 
            dataSet[wordStr] = std::vector<WaveFeatureOP>();

        dataSet[wordStr].push_back(newOP);
    }

protected:
    dataSetType dataSet;
};

#endif
