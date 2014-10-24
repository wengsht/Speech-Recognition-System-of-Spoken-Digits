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
        inline KMeanState *getState(int idx) {
            if(idx >= states.size()) return NULL;
            return (KMeanState *)(states[idx]);
        }

        // 利用path Matrix 更新这个template的分段
        // 对于第idx个template，已经做了dtw，现在更新他在每个state的分段信息
        void updateSegmentation(int templateIdx, int bestFinalRowIdx) {
            int stateIdx = stateNum;
            int rowIdx = bestFinalRowIdx;

            // init
            while(stateIdx >= 1)  {
                getState(stateIdx)->edgePoints[templateIdx] = KMeanState::NullSeg;
                stateIdx --;
            }

            int featureIdx = (*templates)[templateIdx].size() - 1;

            // 分配feature到各个state
            while(featureIdx >= 0) {
                int endIdx = featureIdx;
                int startIdx = featureIdx;

                // 处于统一状态的所有节点
                while(startIdx > 0 && path[startIdx][rowIdx] == rowIdx) {
                    startIdx --;
                }


                getState(rowIdx)->edgePoints[templateIdx] = std::make_pair(startIdx, endIdx);

                featureIdx = startIdx - 1;

                rowIdx = path[startIdx][rowIdx];
            }
        }

        // return true if big change
        bool updateTransfer() {
            double wholeChangeCost = 0.0;

            for(int i = 1; i <= stateNum; i++) {
                //  处于这个state的节点个数
                int wholeCnt = 0;

                int nxtCnt[DTW_MAX_FORWARD];
                memset(nxtCnt, 0, sizeof(nxtCnt));

                // 统计每个template处于这个状态的转移
                for(int j = 0;j < templates->size(); j ++) {
                    std::pair<int,int> seg = getState(i)->edgePoints[j];

                    int numNode = seg.second - seg.first + 1;

                    if(numNode == 0) continue;
                    wholeCnt += numNode;

                    nxtCnt[0] += numNode - 1;

                    // 统计i会转移到i+k 的node的个数
                    for(int k = 1; k < DTW_MAX_FORWARD; k++) {
                        int nxtState = i + k;
                        if(nxtState > stateNum) break;

                        if(getState(nxtState)->edgePoints[j].first - 1 == getState(i)->edgePoints[j].second) {
                            nxtCnt[k] ++;
                            // 一个template只会贡献一个转移
                            break;
                        }
                    }
                }

                if(wholeCnt == 0) return false;

                for(int j = 0;j < DTW_MAX_FORWARD; j++) {
                    double newCost = p2cost(1.0 * nxtCnt[j] / wholeCnt);

                    wholeChangeCost = fabs(newCost - transferCost[i][i+j]);
                    transferCost[i][i+j] = newCost;
                }
            }

            // 每个transfer有0.01的变动 TODO
            return isBigChange(wholeChangeCost);
        }
        bool isBigChange(bool wholeChangeCost) {
            return wholeChangeCost >= 0.01 * stateNum * DTW_MAX_FORWARD;
        }
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
            for(idx = 1;idx <= stateNum;idx++) {
                getState(idx)->gaussianTrain(gaussNum);
            }
            // 
            return bigChange;
        }

        void clearTrainBuffer() {
            path.clear();
            rollColumnCost[0].clear();
            rollColumnCost[1].clear();
        }
};

#endif
