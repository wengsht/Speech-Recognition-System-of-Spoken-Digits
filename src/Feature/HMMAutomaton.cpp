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

HMMAutomaton::HMMAutomaton(std::vector<WaveFeatureOP> *templates, int stateNum, int gaussNum, int trainTimes) : templates(templates), stateNum(stateNum), gaussNum(gaussNum), trainTimes(trainTimes) {
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
    for(int i = 1;i <= stateNum ;i++) 
        for(int j = 1; j <= stateNum ;j++)
            out << " " << transferCost[i][j];

    for(int i = 1;i < states.size(); i++) {
        states[i]->store(out);
    }

}
