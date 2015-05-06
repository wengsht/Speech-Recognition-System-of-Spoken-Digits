// =====================================================================================
// 
//       Filename:  HMMKMeanAutomaton.h
// 
//    Description:  This is KMean Automaton derived from HMMAutomaton
// 
//        Version:  0.01
//        Created:  2014/10/21 22时31分35秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMKMeanAutomaton_H_
#define _AUTOGUARD_HMMKMeanAutomaton_H_

#include <vector>
#include "WaveFeatureOP.h"
#include "configure_hmm.h"
#include "HMMAutomaton.h"
#include "KMeanState.h"
#include "Feature.h"
#include <cstring>
#include "DummyState.h"

class HMMKMeanAutomaton : public HMMAutomaton {
    public:
        friend class HMMSeqKMeanTrainer;
        HMMKMeanAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);
        ~HMMKMeanAutomaton();

        void hmmTrain();

        double calcCost(WaveFeatureOP &input);

        // malloc transfer matrix and hmm states
        void hmmMallocStatesAndTransfer();
        void hmmInitilize();
        void iterateGauss();

        // segment first time
        void initSegment(int templateIdx, int startI, int endI);
        // init transfer
        void initTransfer();

        // every states has not points
        void allEmptyStateSegment();

    private:
        inline KMeanState *getState(int idx) {
            if(idx >= states.size()) return NULL;
            return (KMeanState *)(states[idx]);
        }

        void load(std::stringstream &in) {
            for(int i = 0;i <= stateNum ;i++) 
                for(int j = 0; j <= stateNum ;j++)
                    in >> transferCost[i][j];

            clearStates();

            states.push_back(new DummyState(templates));
            for(int idx = 1;idx <= stateNum;idx ++) {
                states.push_back(new KMeanState(templates));
                states[idx]->load(in, gaussNum);
            }

            // flag models that were loaded from files!!!
            setIsLoadFromFile(true);
        }

        bool updateTransfer();

        void updateSegmentation(int templateIdx, int bestFinalRowIdx);


        //
        // return false to stop training
        bool iterateTrain() {
            int idx, bestFinalRowIdx;

            for(idx = 0; idx < templates->size(); idx++) {
                bestFinalRowIdx = rollDtw((*templates)[idx], HMMAutomaton::Maximum).first;

                // only one path matrix could be used, so update it ight after one dtw
                updateSegmentation(idx, bestFinalRowIdx);
            }

            bool bigChange = updateTransfer();

            // 调整HMM 参数
            iterateGauss();

            return bigChange;
        }

        void clearTrainBuffer() {
            path.clear();
            rollColumnCost[0].clear();
            rollColumnCost[1].clear();
        }
};

#endif
