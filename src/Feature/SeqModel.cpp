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

#define GRAPH_PARAS(type, para) \
    para = graph.get ## para();
#include "graphParas.def"
#undef GRAPH_PARAS

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
                //
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
            head = &(states[from].noEmitHead);
        else 
            head = &(states[from].head);

        edges[edges.size() - 1].next = *head;
        *head = edges.size() - 1;
    }

    assert(edgeCnt == edges.size() - 1);

    SeqEdge &edge = edges[edgeCnt++];

    edge.cost = cost;
}

void SeqModel::recognition(WaveFeatureOP & input, std::vector<std::string> & res, SEQ_DTW_PATH_TYPE path_type) {
    // TO = BACK_PTRDO 

    dtw( input, path_type );

    collectRes(res, path_type, input.size());
}

void SeqModel::collectResFromFullPath(std::vector<std::string> &res, int wavSiz) {
    // check graphParas.def to see define of Termial_States

    if(wavSiz <= 0) return ;

    std::vector< int > path;

    collectBestPath( path, wavSiz );

    assert(path.size() >= wavSiz);

    int stateID = path[0];
    int preStateID = stateID;

    int wavIdx;
    res.push_back( std::string(states[stateID].word ));

    for(wavIdx = 1; wavIdx < wavSiz; wavIdx ++, preStateID = stateID) {
        stateID = path[wavIdx];

        if(0 != strcmp(states[stateID].word, states[preStateID].word)) {
            // new word
            res.push_back( std::string(states[stateID].word ));
        }
    }
}
void SeqModel::collectResFromBackPtr(std::vector<std::string> &res){
    int endState = Terminal_States;
    int backPtrIdx = backPtrs.size() - 1;

    while(backPtrIdx >= 0 && backPtrs[backPtrIdx].stateID != endState) {
        backPtrIdx --;
    }

    assert(backPtrIdx >= 0);

    int *path = new int[backPtrs.size()];
    int wordCnt = 0;

    for(; backPtrIdx >= 0; backPtrIdx = backPtrs[backPtrIdx].prePtr)
        path[wordCnt ++] = backPtrIdx;

    int idx;

    for(idx = 0;idx < wordCnt; idx++) {
        res.push_back( std::string(backPtrs[idx]. word ));
    }

    delete [] path;
}

void SeqModel::collectBestPath( std::vector<int> &path, int wavSiz ) {
    int endState = Terminal_States; 
    int beginState = Start_State; 

    if(path.size() < wavSiz)
        path.resize( wavSiz );

    if(wavSiz <= 0) 
        return ;


    assert( fullPath.size() >= wavSiz );
    assert( fullPath[0].size() >= states.size() );

    int wavIdx = wavSiz - 1;
    int stateID = endState;

    while(!isEmit(stateID) ) 
        stateID = fullPath[wavIdx][stateID];

    for(wavIdx = wavSiz - 1; wavIdx >= 0; wavIdx --) {
        path[wavIdx] = stateID;

        stateID = fullPath[wavIdx][stateID];
        while(!isEmit(stateID) ) 
            stateID = fullPath[wavIdx][stateID];
    }
}

void SeqModel::collectRes(std::vector<std::string> &res, SEQ_DTW_PATH_TYPE path_type, int wavSiz) {
    res.clear();

    switch(path_type) {
        case FULL_PATH:
            collectResFromFullPath(res, wavSiz);
            break;
        case BACK_PTR:
            collectResFromBackPtr(res);
            break;
    }
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
        for(int ei = states[i].noEmitHead; ei != NIL_EDGE; ei = nonEmitEdges[ei].next) {
            out << i << " -> " << nonEmitEdges[ei].to << "[label=\"" << cost2p(nonEmitEdges[ei].cost) << "\"]\n";
        }

    }
    out << "}\n";

}

// BEAM BACKPTR PATH
void SeqModel::dtw(WaveFeatureOP & wav, SEQ_DTW_PATH_TYPE path_type) {
    setPathType(path_type);

    preparePathRecord(wav); 

    Dtw_Column_Link link[2];
    int idx;
    int wavSize = wav.size();
    int stateSiz = states.size();

    link[0].nodes = new Dtw_Column_Node[stateSiz];
    link[1].nodes = new Dtw_Column_Node[stateSiz];

    int columnIdx = -1;
    int rollIdx = getRollIdx(columnIdx);
    int nextIdx;

    link[rollIdx].head = NIL_EDGE;
    link[rollIdx].nodes[0].cost = 0;
    link[rollIdx].addColumnNode(0);
    link[rollIdx].nodes[0].preIdx = NIL_BACK_PTR;

    double bestVal = Feature::IllegalDist;

    for(columnIdx = 0; columnIdx < wavSize; columnIdx ++) {
        bestVal = forwardColumn(link, wav, columnIdx, bestVal + beamThr);
    }

    delete [] link[0].nodes;
    delete [] link[1].nodes;
}

void SeqModel::preparePathRecord(WaveFeatureOP & wav) {
    int idx;
    if(path_type == FULL_PATH) {
        // fullpath[wavSize][stateSize]
        if(fullPath.size() < wav.size()) 
            fullPath.resize(wav.size());

        for(idx = 0; idx < wav.size(); idx ++) {
            if(fullPath[idx].size() < states.size()) 
                fullPath[idx].resize( states.size() );
        }
    }

    else if(path_type == BACK_PTR) {
        backPtrs.clear();
    }
}

double SeqModel::forwardColumn(Dtw_Column_Link *link, WaveFeatureOP & wav, int columnIdx, double threshold) {
    double bestVal = Feature::IllegalDist;
    assert(columnIdx < wav.size());
    int rollIdx = getRollIdx( columnIdx );
    int preIdx = rollIdx ^ 1;
    
    int nextStateID;

    link[rollIdx].clear();

    for(int preStateID = link[preIdx].head; preStateID != NIL_EDGE; preStateID = link[preIdx].nodes[preStateID].next) {
        // beam
        Dtw_Column_Node & dtw_node = link[preIdx].nodes[preStateID];

        double preCost = dtw_node.cost;
        double nextCost;
        if(doBeam && Feature::better(threshold, preCost))
            continue;

        // preStateID --- edges[eID] ---> nextStateID
        for(int eID = states[preStateID].head; eID != NIL_EDGE; eID = edges[eID].next) {
            nextStateID = edges[eID].to;

            nextCost = preCost + edges[eID].cost;

            nextCost += states[nextStateID].hmmState->nodeCost( &(wav[columnIdx]) );

            if(Feature::better(nextCost, bestVal)) 
                bestVal = nextCost;

            tryUpdate(link, columnIdx, preStateID, nextStateID, nextCost);
        }
    }

    //
    // non-emit state inner forward
    //
    for(int noEmitStateID = 0; noEmitStateID < N_States; noEmitStateID ++) {
        Dtw_Column_Node & dtw_node = link[rollIdx].nodes[noEmitStateID];

        double preCost = dtw_node.cost;
        double nextCost;

        // preStateID --- edges[eID] ---> nextStateID
        for(int eID = states[noEmitStateID].noEmitHead; eID != NIL_EDGE; eID = nonEmitEdges[eID].next) {
            nextStateID = nonEmitEdges[eID].to;

            // penalty
            nextCost = preCost + edges[eID].cost;

            tryUpdate(link, columnIdx, noEmitStateID, nextStateID, nextCost);
        }

    }

    return bestVal;
}

void SeqModel::tryUpdate(Dtw_Column_Link * link, int columnIdx, int preStateID, int stateID, double newCost) {
    int rollIdx = getRollIdx(columnIdx);
    int preIdx = rollIdx ^ 1;

    Dtw_Column_Node & dtw_node = link[rollIdx].nodes[stateID];

    double updateFlag = false;

    // new node!!
    if(dtw_node.lastUpdate != columnIdx) {
        link[rollIdx].addColumnNode( stateID );

        updateFlag = true;
    }
    else if(Feature::better(newCost, dtw_node.cost)) 
        updateFlag = true;

    if(updateFlag) {
        dtw_node.cost = newCost;

        tryForwardToStar( link, columnIdx, stateID, newCost );

        // record path 
        if(path_type == FULL_PATH) {
            fullPath[columnIdx][stateID] = preStateID;
        }
        else if(path_type == BACK_PTR) {
            if(isEmit(preStateID) && isEmit(stateID)) {
                dtw_node.preIdx = link[preIdx].nodes[preStateID].preIdx;
            }
            else if(!isEmit(preStateID) && !isEmit(stateID) ) {
                dtw_node.preIdx = link[rollIdx].nodes[preStateID].backPtr;
            }
            else if(!isEmit(preStateID) && isEmit(stateID) ) {
                dtw_node.preIdx = link[preIdx].nodes[preStateID].backPtr;
            }
            else if(isEmit(preStateID) && ! isEmit(stateID) ) {
                // new back ptr
                // should be forwarding

                // same column, same rollIdx , forwarding

                dtw_node.preIdx = link[rollIdx].nodes[preStateID].preIdx;

                if(dtw_node.backPtr == NIL_BACK_PTR) {
                    backPtrs.push_back( BackPtr() );
                    dtw_node.backPtr = backPtrs.size() - 1;
                }

                backPtrs[dtw_node.backPtr].word = states[preStateID].word;
                backPtrs[dtw_node.backPtr].prePtr = dtw_node.preIdx;
                backPtrs[dtw_node.backPtr].stateID = stateID;
            }
        }
    }
}

void SeqModel::tryForwardToStar( Dtw_Column_Link *link, int columnIdx, int stateID, double preCost) {
    if( states[stateID].leafForwardIdx == NIL_FORWARD ) 
        return ;

    double newCost = preCost + states[stateID].forwardCost;

    tryUpdate(link, columnIdx, stateID, states[stateID].leafForwardIdx, newCost);
}
