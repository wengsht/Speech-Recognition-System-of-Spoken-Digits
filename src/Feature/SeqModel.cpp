// =====================================================================================
// 
//       Filename:  SeqModel.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/18 22时54分25秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "SeqModel.h"
#include "NoEmitState.h"
#include "mathtool.h"
#include <iostream>
#include "Feature.h"

void SeqModel::buildModel(const ParseGraph & graph, std::map< std::string, HMMAutomaton *> & automatons) {
//    graph.dump(std::cout);

    initNOEmitStates( graph );

    refreshEdges( graph, automatons );

    /*  
    for(int idx = 0; idx < graph.size(); idx++) {
        addEdge( graph[idx], automatons );
    }
    */
}

void SeqModel::refreshEdges( const ParseGraph & graph, std::map< std::string, HMMAutomaton *> & automatons) {
    int edgeCnt = 0;
    int nonEmitEdgeCnt = 0;
    int stateCnt = N_States;

    for(int idx = 0; idx < graph.size(); idx++) {
        refreshEdge(graph[idx], automatons, edgeCnt, stateCnt, nonEmitEdgeCnt);
    }
}

void SeqModel::initNOEmitStates( const ParseGraph & graph ) {
    close();

    N_States = graph.getN_States();
    Start_State = graph.getStart_State();
    
    for(int idx = 0; idx < graph.getN_States(); idx ++) {
        SeqState noEmit;
        noEmit.hmmState = new NoEmitState();
        states.push_back( noEmit );
    }
}

void SeqModel::refreshEdge(const GraphEdge & simpleEdge, std::map< std::string, HMMAutomaton *> & automatons, int &edgeCnt, int &stateCnt, int &nonEmitEdgeCnt) {
    const int &from = simpleEdge.from;
    const int &to   = simpleEdge.to;
    const double &probability = simpleEdge.probability;
    const std::string &word = simpleEdge.word;

    assert(from < N_States);
    assert(to < N_States);

    int idx, idy, idz;
    int b_from, b_to;

    std::map< std::string, HMMAutomaton *>::iterator Itr;
    Itr = automatons.find(word);

    // hmm edge
    if(Itr != automatons.end()) {
        HMMAutomaton & automaton = *(Itr->second);
        // start-into-hmm
        // hmm-to-hmm
        
        int baseEsp = stateCnt - 1;
        for(idx = 1; idx <= automaton.getStateNum(); idx ++) {
            // idx -> idx + 1
            refreshNode(automaton, idx, stateCnt, Itr->first);
        }

        for(idx = 0; idx <= automaton.getStateNum(); idx ++) {
            for(idz = 0; idz < DTW_MAX_FORWARD; idz ++) {
                idy = idx + idz;
                if(idy > automaton.getStateNum())
                    break;

                //
                // dont add self-edge on no-emit state
                //
                if(!idx && !idy) continue;

                b_from = idx + baseEsp;
                b_to   = idy + baseEsp;
                if(0 == idx) b_from = from;
                if(0 == idy) b_to   = from;

                double cost = automaton.transferCost[idx][idy];
                // 0->  要加上单词间的penalty
                //
                if(!idx) 
                    cost = cost + p2cost(probability);

                refreshEdge(edges, b_from, b_to, cost, edgeCnt);
            }
        }
        
        //
        // hmm-to-end

        for(int idx = automaton.getStateNum() - DTW_MAX_FORWARD + 2; idx <= automaton.getStateNum(); idx ++) {
            double cost = p2cost(automaton.enddingProbability(idx));

            b_from = idx + baseEsp;
            b_to   = to;

            // forward edges will not be stored in edge table
            states[b_from].leafForwardIdx = b_to;
            states[b_from].forwardCost = cost;
//            refreshEdge(b_from, b_to, cost, edgeCnt);
        }
    }
    // penalty
    else {
        refreshEdge(nonEmitEdges, from, to, p2cost(probability), nonEmitEdgeCnt);
    }
}

int SeqModel::refreshNode( const HMMAutomaton & automaton, int stateID, int &stateCnt, const std::string &word) {
    assert(stateID >= 1 && stateID <= automaton.getStateNum());

    if(stateCnt < states.size())
        return stateCnt++;

    states.push_back( SeqState() );
    assert(stateCnt == states.size() - 1);

    states[stateCnt].hmmState = automaton.states[stateID];
    states[stateCnt].word = word.c_str();

    return stateCnt++;
}

void SeqModel::refreshEdge(std::vector<SeqEdge> &edges, int from, int to, double cost, int &edgeCnt) {
    if(edgeCnt >= edges.size()) {
        edges.push_back(SeqEdge());
        edges[edges.size() - 1].to = to;

        int *head;

        // if from and to are all non emit state, add to non emit table
        if(from < N_States && to < N_States) 
            head = &(states[from].emitHead);
        else 
            head = &(states[from].head);

        edges[edges.size() - 1].next = *head;
        *head = edges.size() - 1;
    }

    assert(edgeCnt == edges.size() - 1);

    SeqEdge &edge = edges[edgeCnt++];

    edge.cost = cost;
}

void SeqModel::recognition(WaveFeatureOP & input, std::vector<std::string> & res) {
    // TODO 
}

void SeqModel::close() {
    for(int i = 0;i < states.size(); i++) {
        NoEmitState * tmp = dynamic_cast<NoEmitState *> (states[i].hmmState);

        if(tmp) 
            delete tmp;
        else break;
    }
    states.clear();
    edges.clear();
    nonEmitEdges.clear();
}

void SeqModel::setBeam(double thr) {
    beamThr = thr;
    doBeam = true;
}

SeqModel::SeqModel() : doBeam(false) {

}

SeqModel::~SeqModel() {
    
}

void SeqModel::dumpDot(std::ostream &out) {

    out << "digraph {\n";

    for(int i = 0;i < states.size(); i++) {
        for(int ei = states[i].head; ei != NIL_EDGE; ei = edges[ei].next) {
            out << i << " -> " << edges[ei].to << "[label=\"" << cost2p(edges[ei].cost) << "\"]\n";
        }
        if(states[i].leafForwardIdx != NIL_FORWARD)
            out << i << " -> " << states[i].leafForwardIdx << "[label=\"" << cost2p(states[i].forwardCost) << "\"]\n";
    }
    for(int i = 0;i < N_States; i++) {
        for(int ei = states[i].emitHead; ei != NIL_EDGE; ei = nonEmitEdges[ei].next) {
            out << i << " -> " << nonEmitEdges[ei].to << "[label=\"" << cost2p(nonEmitEdges[ei].cost) << "\"]\n";
        }

    }
    out << "}\n";

}

// BEAM BACKPTR PATH
void SeqModel::dtw(WaveFeatureOP & wav, SEQ_DTW_PATH_TYPE path_type) {
    Dtw_Column_Link link[2];
    int idx;
    int wavSize = wav.size();
    int stateSiz = states.size();

    link[0].nodes = new Dtw_Column_Node[stateSiz];
    link[1].nodes = new Dtw_Column_Node[stateSiz];

    int columnIdx = -1;
    int rollIdx = getRollIdx(columnIdx);

    link[rollIdx].head = NIL_EDGE;
    // set init todo

    double bestVal = Feature::IllegalDist;

    for(columnIdx = 0; columnIdx < wavSize; columnIdx ++) {

    }

    delete [] link[0].nodes;
    delete [] link[1].nodes;
}
