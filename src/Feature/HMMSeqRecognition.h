// =====================================================================================
// 
//       Filename:  HMMSeqRecognition.h
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/18 21时41分57秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMSeqRecognition_H_
#define _AUTOGUARD_HMMSeqRecognition_H_

#include "HMMRecognition.h"
#include "configure_hmm.h"
#include "HMMAutomaton.h"
#include "parseGraph.h"
#include "SeqModel.h"
#include <vector>

class HMMSeqRecognition : public HMMRecognition {
    public:
        HMMSeqRecognition(int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);
        ~HMMSeqRecognition();

        // segmented word based train
        void hmmSegTrain();
        // sequential based train
        void hmmSeqTrain();

        void loadGraph(const char *filename);

        void recognition(WaveFeatureOP & input, std::vector<std::string> & res);

        void setBeam( double beam );

        void close();

    private:
        void buildSeqModel();
        ParseGraph graph;
        SeqModel seqModel;
};

#endif
