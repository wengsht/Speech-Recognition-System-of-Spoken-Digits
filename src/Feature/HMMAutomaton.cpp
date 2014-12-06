// =====================================================================================
// 
//       Filename:  HMMAutomaton.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/21 22时33分18秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "HMMAutomaton.h"

HMMAutomaton::HMMAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum, int gaussNum, int trainTimes) : templates(templates), stateNum(stateNum), gaussNum(gaussNum), trainTimes(trainTimes), loadFlag(false) {
    // 转移矩阵什么的开空间
    Init();
}


void HMMAutomaton::close() {
    clearStates();
}
void HMMAutomaton::clearStates() {
    int idx;
    for(idx = 0; idx < states.size(); idx++)  {
        delete states[idx];
    }

    states.clear();

}

void HMMAutomaton::store(std::stringstream &out) {
    for(int i = 0;i <= stateNum ;i++) 
        for(int j = 0; j <= stateNum ;j++)
            out << " " << transferCost[i][j];


    for(int i = 1;i < states.size(); i++) {
        states[i]->store(out);
    }
}

void HMMAutomaton::dumpTransfer(std::ostream & out) {
    int siz = transferCost.size();
    if(!siz) return ;

    for(int i = 0;i < transferCost.size(); i++) {
        for(int j = 0;j < transferCost[i].size(); j++) {
            double p = cost2p(transferCost[i][j]);
            if(i <= j && i+2 >= j)
                out << i << " -> " << j << "[label=\"" << p << "\", weight=\"" << p << "\"]\n" ;
        }
    }

    for(int i = stateNum - DTW_MAX_FORWARD + 2; i <= stateNum; i++) {
        double p = enddingProbability(i); //1.0-cost2p(transferCost[siz-2][siz-2]) -cost2p(transferCost[siz-2][siz-1]);
        if(p < FLOOR_TRANSITION_PROBABILITY) 
            continue;

        out << i << " -> " << "dummy_end" << "[label=\"" << p <<  "\", weight=\"" << p << "\"]\n" ;

    }
    /*  
    double p1 = enddingProbability(siz-2); //1.0-cost2p(transferCost[siz-2][siz-2]) -cost2p(transferCost[siz-2][siz-1]);

    double p2 = enddingProbability(siz-1); //1.0-cost2p(transferCost[siz-1][siz-1]);

    out << siz-2 << " -> " << "dummy_end" << "[label=\"" << p1 <<  "\", weight=\"" << p1 << "\"]\n" ;

    out << siz-1 << " -> " << "dummy_end" << "[label=\"" << p2 <<  "\", weight=\"" << p2 << "\"]\n" ;
    */
}

double HMMAutomaton::enddingProbability(int stateID) {
    assert(stateID <= stateNum);

    double res = 1.0;
    for(int nxt = stateID; nxt <= stateNum; nxt ++) 
        res -= cost2p(transferCost[stateID][nxt]);

//    if(stateID < stateNum)
//        res -= cost2p(transferCost[stateID][stateID + 1]);

    return res;
}

void HMMAutomaton::adjustSkippingTransfer() {
    int siz = transferCost.size();
    if(siz <= 0) return ;

    for(int i = 0;i < siz-2;i++) {
        if(cost2p(transferCost[i][i+2]) < FLOOR_TRANSITION_PROBABILITY) {
            transferCost[i][i+2] = p2cost(cost2p(transferCost[i][i+2]) + FLOOR_TRANSITION_PROBABILITY);

            if(i)
                transferCost[i][i] = p2cost(cost2p(transferCost[i][i]) - FLOOR_TRANSITION_PROBABILITY);
            else {
                transferCost[i][i+1] = p2cost(cost2p(transferCost[i][i+1]) - FLOOR_TRANSITION_PROBABILITY);
            }
        }
    }
}
