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
    loadSpecStateNum();
}
HMMAutomatonSet::~HMMAutomatonSet() {

}
void HMMAutomatonSet::loadSpecStateNum() {
#define SpecificStateNum(word, num) \
    specStateNums[word] = num;
#include "SpecificStateNum.def"
#undef  SpecificStateNum
}


void HMMAutomatonSet::hmmTrainTask(void *in) {
    HMMAutomaton * automaton = (HMMAutomaton *) in;

    automaton->hmmTrain();
}

int HMMAutomatonSet::getSpecificStateNum( std::string &word ) {
//    return stateNum;
    if(specStateNums.count(word))
        return specStateNums[word];

    return stateNum;
}

void HMMAutomatonSet::reGenerateAutomaton() {
    dataSetType::iterator templateItr;
    for(templateItr = dataSet.begin(); templateItr != dataSet.end(); templateItr ++) {
        std::string word = templateItr->first;

        if(automatons.count(word))
            continue;

        int specificStateNum = getSpecificStateNum( word );
        // 新建自动机
        if(stateType == HMMState::KMEAN)
            automatons[word] = new HMMKMeanAutomaton(&(templateItr->second), specificStateNum, gaussNum, trainTimes); 
        else 
            automatons[word] = new HMMSoftAutomaton(&(templateItr->second), specificStateNum, gaussNum, trainTimes); 
    }
}
SP_RESULT HMMAutomatonSet::train() {
    reGenerateAutomaton();

    ThreadPool threadPool(ThreadPool::thread_num);
    std::map< std::string, HMMAutomaton *>::iterator Itr;
    for(Itr = automatons.begin(); Itr != automatons.end(); Itr++) {
        std::string word = Itr->first;
        sp_task task;

        task.func = hmmTrainTask;
        task.in   = (void *)(Itr->second);

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

void HMMAutomatonSet::load(std::ifstream &in) {
    reGenerateAutomaton();
    std::string word;

    std::string tmp;
    while(getline(in, tmp)) {
        std::stringstream sin(tmp);

        sin >> word;

        automatons[word]->load(sin);
    }
}

void HMMAutomatonSet::store(std::ofstream &out) {
    std::map< std::string, HMMAutomaton *>::iterator Itr;
    for(Itr = automatons.begin(); Itr != automatons.end(); Itr ++) {
        std::string tmp("");
        std::stringstream sout(tmp);
        sout << Itr->first << " ";

        (Itr->second)->store(sout);

        out << sout.str() << std::endl;
    }
}
void HMMAutomatonSet::dumpAutomaton(std::ostream & out) {
    std::map< std::string, HMMAutomaton *>::iterator Itr;
    for(Itr = automatons.begin(); Itr != automatons.end(); Itr ++) {
        out << "digraph {\n";
        out << Itr->first << ";\n";
        (Itr->second)->dumpTransfer(out);
        out << "}\n";
    }
}
