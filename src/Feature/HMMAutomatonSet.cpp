// =====================================================================================
// 
//       Filename:  HMMAutomatonSet.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/21 21时49分31秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "HMMAutomatonSet.h"
#include "HMMKMeanAutomaton.h"
#include "HMMSoftAutomaton.h"
#include <string>
#include <iostream>
#include "ThreadPool.h"

HMMAutomatonSet::HMMAutomatonSet(int stateNum, int gaussNum, int trainTimes) : stateNum(stateNum), gaussNum(gaussNum), trainTimes(trainTimes) {
}
HMMAutomatonSet::~HMMAutomatonSet() {
}


void HMMAutomatonSet::hmmTrainTask(void *in) {
    HMMAutomaton * automaton = (HMMAutomaton *) in;

    automaton->hmmTrain();
}

SP_RESULT HMMAutomatonSet::train() {
    clear();

    dataSetType::iterator templateItr;

    ThreadPool threadPool(ThreadPool::thread_num);
    for(templateItr = dataSet.begin(); templateItr != dataSet.end(); templateItr ++) {
        std::string word = templateItr->first;

        if(automatons.count(word))
            continue;
        // 新建自动机
        if(stateType == HMMState::KMEAN)
            automatons[word] = new HMMKMeanAutomaton(&(templateItr->second), stateNum, trainTimes); 
        else 
            automatons[word] = new HMMSoftAutomaton(&(templateItr->second), stateNum, trainTimes); 

        sp_task task;

        task.func = hmmTrainTask;
        task.in   = (void *)(automatons[word]);

        threadPool.addTask(task);

//        automatons[word]->hmmTrain();
    }
    threadPool.run();
    return SP_SUCCESS;
}

std::string HMMAutomatonSet::recognition(WaveFeatureOP & input) {
    std::map< std::string, HMMAutomaton *>::iterator Itr;
    std::string res = "null2";
    
    double bestCost = Feature::IllegalDist;
    for(Itr = automatons.begin(); Itr != automatons.end(); Itr ++) {
        double tmpCost = Itr->second->calcCost(input);

        // DEBUG

//        std::cout << Itr->first << " " << tmpCost << std::endl;
        if(Feature::better(tmpCost, bestCost)) {
            res = Itr->first;
            bestCost = tmpCost;
        }
    }
    return res;
}

void HMMAutomatonSet::clear() {
    std::map< std::string, HMMAutomaton *>::iterator Itr;

    for(Itr = automatons.begin(); Itr != automatons.end(); Itr ++) {

        (Itr->second)->close();

        delete (Itr->second);
    }
    automatons.clear();
}
