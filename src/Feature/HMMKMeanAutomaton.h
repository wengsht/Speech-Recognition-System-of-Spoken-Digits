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

class HMMKMeanAutomaton : public HMMAutomaton {
    public:
        HMMKMeanAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);
        ~HMMKMeanAutomaton();

        void hmmTrain();

        double calcCost(WaveFeatureOP &input);
    private:
        double dtw(WaveFeatureOP &input, bool recordPath) {
            /* 
            if(recordPath)
                recordInit();

            int columnIdx = 0;
            if(!input.size()) return Feature::IlledgeDist;

            int rollIdx = getRollIdx(columnIdx);

            int idx, idy;

            // 第一列只从dummy扩展
            for(idx = 1; idx <= stateNum; idx ++) {
                int stateIdx = getStateIdx(idx);

                rollColumnCost[rollIdx][stateIdx] = transferCost[0][idx] + states[stateIdx].nodeCost(&input[0]);
            }

            for(columnIdx = 1; columnIdx < input.size(); columnIdx = ) {

            }
            */


            return 1.0;
        }
        int getBestRow(int columnIdx) {
            int rollIdx = getRollIdx(columnIdx);

            int res = 1;
            for(int idx = 2; idx < stateNum; idx++) 
                if(Feature::better(rollColumnCost[rollIdx][idx], rollColumnCost[rollIdx][res]))
                    res = idx;

            return res;
        }
        // 利用path Matrix 更新这个template的分段
        void updateSegmentation(int featureIdx) {
            int bestRow = getBestRow(templates[featureIdx].size() - 1); // 得到最后处于哪个状态

            int idx = stateNum;
            // 没有节点处于最后那些状态！！
            while(idx > bestRow) {
                int stateIdx = getStateIdx(idx);
                states[stateIdx].edgePoints[featureIdx] = std::make_pair(0, -1);
                idx --;
            }

            int featureCnt = templates[featureIdx].size() - 1;

            while(featureCnt >= 0) {
                int stateIdx = getStateIdx(idx);
                int endColumn = featureCnt;
                
                while(featureCnt && path[featureCnt][bestRow] == bestRow) {
                    featureCnt --;
                }

                states[stateIdx].edgePoints[featureIdx] = std::make_pair(featureCnt, endColumn);

                idx --;
            }

            while(idx >= 1) {
                int stateIdx = getStateIdx(idx);
                states[stateIdx].edgePoints[featureIdx] = std::make_pair(0, -1);
                idx --;
            }
        }
        void updateTransfer() {

        }
        // return false to stop training
        bool iterateTrain() {
            int idx;
            for(idx = 0; idx < templates->size(); idx++) {
                dtw((*templates)[idx], true);

                updateSegmentation(idx);
            }

            // 调整HMM 参数
            for(idx = 1;idx <= stateNum;idx++) {
                int stateIdx = getStateIdx(idx);

                states[stateIdx].gaussianTrain(gaussNum);
            }

            updateTransfer();
        }
        std::vector<KMeanState> states;
};

#endif
