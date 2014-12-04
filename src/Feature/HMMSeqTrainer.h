// =====================================================================================
// 
//       Filename:  HMMSeqTrainer.h
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/19 02时13分42秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMSeqTrainer_H_
#define _AUTOGUARD_HMMSeqTrainer_H_

#include <vector>
#include "parseGraph.h"
#include "WaveFeatureOPSet.h"
#include "WaveFeatureOP.h"
#include "SeqModel.h"
#include "HMMAutomaton.h"

struct SeqWav {
//    SeqWav(WaveFeatureOP * wav, SeqModel * model) : wav(wav), model(model) {}

    WaveFeatureOP * wav;
    SeqModel* model;
};
class HMMSeqTrainer {
    public:
        HMMSeqTrainer();
        virtual ~HMMSeqTrainer();


        // 对dataSet每一个单词序列建Graph， 并把所有wavs dump搭配mixedWavs, 同时维护mixedWavs 到graph的映射
        void buildModels(WaveFeatureOPSet::dataSetType & dataSet, std::map<std::string, HMMAutomaton *> &automatons, std::vector<WaveFeatureOP> &mixedWavs);

    private:
        void pushNewModel(const std::string &seqStr,std::map<std::string, HMMAutomaton *> &automatons, std::vector<SeqModel> &models);

        virtual void hmmSeqTrain() = 0;

    protected:
        struct RefreshInfo {
            SeqModel * model;
            ParseGraph * graph;
            std::map<std::string, HMMAutomaton *> * automatons;
        };
        struct BuildInfo {
            SeqModel * model;
            ParseGraph * graph;
            std::map<std::string, HMMAutomaton *> * automatons;
        };

        static void refreshModelTask(void *in);
        static void buildModelTask(void *in);

        void refreshModels();

        void buildModels();
    protected:
        std::map<std::string, HMMAutomaton *> *automatons;

        std::vector< HMMAutomaton *> automatonVec;

        std::vector<WaveFeatureOP> * mixedWavs;

        std::vector<SeqModel> trainModels;

        std::vector<ParseGraph> graphs;

        std::vector<SeqWav> trainWavs;
};

#endif

