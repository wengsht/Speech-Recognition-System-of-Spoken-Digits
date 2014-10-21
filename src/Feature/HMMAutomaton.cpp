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

HMMAutomaton::HMMAutomaton(std::vector<WaveFeatureOP> *features, int stateNum, int gaussNum) : features(features), stateNum(stateNum), gaussNum(gaussNum) {

}
