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
        HMMKMeanAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum = AUTOMATON_STATE_NUM, int gaussNum = GAUSSIAN_NUM, int trainTimes = MAX_TRAIN_TIMES);
        ~HMMKMeanAutomaton();

        void hmmTrain();

        double calcCost(WaveFeatureOP &input);

        void hmmInitilize();

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

                // 处于同一状态的所有节点
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
            double wholeChangeCost = Feature::IllegalDist;

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
//                    if(seg.second == (*templates)[j].size()-1)
//                        nxtCnt[0] += 1;

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

                if(wholeCnt == 0) {
                    continue;
                }

                for(int j = 0;j < DTW_MAX_FORWARD; j++) {
                    if(i+j > stateNum) break;
                    double newCost = p2cost(1.0 * nxtCnt[j] / wholeCnt);

                    wholeChangeCost= logInsideSum(wholeChangeCost, logInsideDist(newCost, transferCost[i][i+j]));
                    //                    wholeChangeCost = fabs(newCost - transferCost[i][i+j]);

                    transferCost[i][i+j] = newCost;
                }
            }

            if(stateNum >= 2) {
                int beginSkipCnt = 0;
                for(int j = 0; j < templates->size(); j++) {
                    std::pair<int,int> seg = getState(1)->edgePoints[j];
                    int numNode = seg.second - seg.first + 1;

                    if(numNode == 0) beginSkipCnt ++;
                }

                transferCost[0][1] = p2cost(1.0 * (templates->size() - beginSkipCnt) / templates->size());
                transferCost[0][2] = p2cost(1.0 * (beginSkipCnt) / templates->size());
            }

            adjustSkippingTransfer();

            // 每个transfer有0.01的变动 TODO
            return isBigChange(wholeChangeCost);
        }
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
