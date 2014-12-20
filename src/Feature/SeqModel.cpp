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
#include "SerialFiles.h"
#include "KMeanState.h"

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
//    printf("%d %d\n", from, to);
    const double &probability = simpleEdge.probability;
    const std::string &word = simpleEdge.word;

    assert(from < N_States);
    assert(to < N_States);

    int idx, idy, idz;
    int b_from, b_to;

    std::map< std::string, HMMAutomaton *>::iterator Itr;
    Itr = automatons.find(word);

//    int edgeUUID = edgeCnt;

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
                    //cost = cost + p2cost(1e-65);

                refreshEdge(edges, b_from, b_to, cost, edgeCnt);
            }
        }
        
        //
        // hmm-to-end

        for(int idx = automaton.getStateNum() - DTW_MAX_FORWARD + 2; idx <= automaton.getStateNum(); idx ++) {
            double p = automaton.enddingProbability(idx);
            if(p < FLOOR_TRANSITION_PROBABILITY)
                continue;

            double cost = p2cost(p);

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
        if(! isEmit(from) && ! isEmit(to))
//        if(from < N_States && to < N_States) 
            head = &(states[from].noEmitHead);
        else 
            head = &(states[from].head);

        edges[edges.size() - 1].next = *head;

        *head = edges.size() - 1;
    }

    assert(edgeCnt < edges.size());

    SeqEdge &edge = edges[edgeCnt++];

    edge.cost = cost;
}

void SeqModel::recognition(WaveFeatureOP & input, std::vector<std::string> & res, SEQ_DTW_PATH_TYPE path_type) {
//    path_type = FULL_PATH;
    dtw( input, path_type );

    collectRes(res, path_type, input.size());
}

void SeqModel::reSegment(WaveFeatureOP & input, int templateIdx) {
    dtw( input, FULL_PATH );
//    printf("%p\n", this);

    std::vector< int > path;
    collectBestPath( path, input.size() );
    if(path.size() <= 0) {
        Warn("Don't try to train a empty file!");
        return ;
    }

    int pathIdx;

    int preState = path[0];
    int stateID;
    int startI = 0;

    KMeanState *state;
    for(pathIdx = 1; pathIdx < path.size(); pathIdx ++) {
        stateID = path[pathIdx];
//        Log("%d->%d : %d ", preState, stateID, pathIdx);

        if(stateID != preState) {
            // no support soft yet..
            state = (KMeanState *)(states[preState].hmmState);

//            Log("State %d: %d->%d\n", preState, startI, pathIdx - 1);
            state->edgePoints[templateIdx] = std::make_pair(startI, pathIdx - 1);


            preState = stateID;
            startI = pathIdx;
        }
    }
    state = (KMeanState *)(states[preState].hmmState);

//    Log("State %d: %d->%d\n", preState, startI, input.size()- 1);
    state->edgePoints[templateIdx] = std::make_pair(startI, input.size() - 1);

//        printf("xxoo %d\n", ((KMeanState *)(states[15].hmmState))->points.size());
}

void SeqModel::collectResFromFullPath(std::vector<std::string> &res, int wavSiz) {
    // check graphParas.def to see define of Termial_States

    if(wavSiz <= 0) return ;

    std::vector< int > path;

    collectBestPath( path, wavSiz, true);

    assert(path.size() >= wavSiz);

    int stateID = path[0];

    int wavIdx;

    /*  
    if(states[stateID].leafForwardIdx != NIL_FORWARD) {
        res.push_back( std::string(states[stateID].word ));
    }
    */

//    puts("");
//        printf("%d ", stateID);
    for(wavIdx = 0; wavIdx < wavSiz; wavIdx ++) {
        stateID = path[wavIdx];

//        printf("%d ", stateID);
               if(stateID != NIL_EDGE) {
                   res.push_back( std::string(states[stateID].word ));

        }
    }
//    puts("");
}

void SeqModel::collectResFromBackPtr(std::vector<std::string> &res) {
    int endState = Terminal_States;
    int backPtrIdx = backPtrs.size() - 1;

    while(backPtrIdx >= 0 && backPtrs[backPtrIdx].stateID != endState) {
        backPtrIdx --;
    }

    /*  
    for(int j = backPtrIdx; j >= 0; j--) {
        if(backPtrs[j].stateID == endState) {
            int kk = j;

            for(; kk >= 0; kk = backPtrs[kk].prePtr)  {
                if(! SerialFiles::isNotWord(const_cast<char *>(backPtrs[kk].word)))
                    printf("%s ", backPtrs[kk].word );
            }
            puts("");
        }
    }
    */

//    assert(backPtrIdx >= 0);

    if(backPtrIdx < 0) {
        Log("No model was found!! Try set bigger beam thres");
        return ;
    }

    int *path = new int[backPtrs.size()];
    int wordCnt = 0;
 
    for(; backPtrIdx >= 0; backPtrIdx = backPtrs[backPtrIdx].prePtr) 
        path[wordCnt ++] = backPtrIdx;

    int idx;

    for(idx = wordCnt - 1;idx >= 0; idx--) {
//        printf("%d ", backPtrs[path[idx]].stateID);
        res.push_back( std::string(backPtrs[ path[idx] ].word ));
    }
//    puts("");

    delete [] path;
}

void SeqModel::collectBestPath( std::vector<int> &path, int wavSiz , bool eliminateDuplicate) {
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

    bool newWord = true;
    for(wavIdx = wavSiz - 1; wavIdx >= 0; wavIdx --) {
        while(!isEmit(stateID)  && stateID != fullPath[wavIdx][stateID])  {
            stateID = fullPath[wavIdx][stateID];

            newWord = true;
        }

        path[wavIdx] = stateID;

        if(!isEmit(stateID) || (eliminateDuplicate && !newWord))
            path[wavIdx] = NIL_EDGE;

        newWord = false;

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
        default:
            break;
    }
}

void SeqModel::close() {
    for(int i = 0;i < states.size(); i++) {
        // emit state is manage by hmm model
        if(! isEmit(i))
            delete states[i].hmmState;

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
    int start = Start_State;

    link[0].nodes = new Dtw_Column_Node[stateSiz];
    link[1].nodes = new Dtw_Column_Node[stateSiz];

    int columnIdx = -1;
    int rollIdx = getRollIdx(columnIdx);

    link[rollIdx].head = NIL_EDGE;

    link[rollIdx].nodes[start].cost = 0;
    link[rollIdx].addColumnNode(start);
    link[rollIdx].nodes[start].preIdx = NIL_BACK_PTR; // first backPtr is actually -1(don't exist)

    // forward start state to non-emit states if needed
    for(int eID = states[start].noEmitHead; eID != NIL_EDGE; eID = nonEmitEdges[eID].next) {
        int nextStateID = nonEmitEdges[eID].to;
        if(nextStateID == start) 
            continue;

        double nextCost = link[rollIdx].nodes[start].cost + nonEmitEdges[eID].cost;

        link[rollIdx].nodes[nextStateID].cost = nextCost;
        link[rollIdx].addColumnNode(nextStateID);
        link[rollIdx].nodes[nextStateID].preIdx = NIL_BACK_PTR; // first backPtr is actually -1(don't exist)
    }


    double bestVal = Feature::IllegalDist;

    for(columnIdx = 0; columnIdx < wavSize; columnIdx ++) {
//        Log(GREEN "Forwarding column [%d]\n", columnIdx);

        bestVal = forwardColumn(link, wav, columnIdx, bestVal + beamThr);

//        printf("%d %lf\n", columnIdx, bestVal);

//        Log("RED %lf", link[getRollIdx(columnIdx)].nodes[Terminal_States].cost);
        /*  
        if(link[getRollIdx(columnIdx)].nodes[7].lastUpdate == columnIdx)
            printf(RED "%d %d\n", columnIdx, link[getRollIdx(columnIdx)].nodes[7].preIdx);
            */
    }

    Tip(BLUE "Best Cost: %lf\n" , link[getRollIdx(wavSize - 1)].nodes[Terminal_States].cost);

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

//    assert(columnIdx < wav.size());
    if(columnIdx >= wav.size()) {
        Warn("Forwarding a columnIdx larger than wav SIZE!");
        return bestVal;
    }

    int rollIdx = getRollIdx( columnIdx );
    int preIdx = rollIdx ^ 1;

    int nextStateID;

    link[rollIdx].clear();

    for(int preStateID = link[preIdx].head; \
            preStateID != NIL_EDGE; \
            preStateID = link[preIdx].nodes[preStateID].next) {
        Dtw_Column_Node & dtw_node = link[preIdx].nodes[preStateID];

        double preCost = dtw_node.cost;
        double nextCost;

        // beam
        if(doBeam && Feature::better(threshold, preCost)) {
//            Log("beam %lf", threshold);
            continue;
        }

        // preStateID --- edges[eID] ---> nextStateID
        for(int eID = states[preStateID].head; \
                eID != NIL_EDGE; \
                eID = edges[eID].next) {
            nextStateID = edges[eID].to;

//            Log("Edge Cost: %lf\n", edges[eID].cost);

            nextCost = preCost + edges[eID].cost;

            /*  
        if(!(nextCost>= 0) && !(nextCost<= 0)) {
                printf("%lf %lf %lf\n", nextCost,preCost, edges[eID].cost);
                getchar();
        }
        */


            KMeanState * state = (KMeanState *)(states[nextStateID].hmmState);
            double nodeCost = state->nodeCost( &(wav[columnIdx]) );

            nextCost += nodeCost;

//            Log("Node Cost: %lf\n", nodeCost);
//            Log("Cost: %lf\n", nextCost);

            if(Feature::better(nextCost, bestVal)) 
                bestVal = nextCost;

            tryUpdate(link, columnIdx, preStateID, nextStateID, nextCost);
        }
    }

    //
    // non-emit state inner forward
    //
    // it is a columnIdx -> columnIdx forward
    //
    // TODO : should i flood fill ?

    // 这里因为non-emit之间的forward只能用旧值，不能用nonemit forward过来的值去更新另一个non-emit， 因此要用另一个link存旧值
    // 但本质上是 同一时间t的forward操作
    for(int noEmitStateID = 0; noEmitStateID < N_States; noEmitStateID ++) {
        link[preIdx].nodes[noEmitStateID] = link[rollIdx].nodes[noEmitStateID];
    }

    for(int noEmitStateID = 0; noEmitStateID < N_States; noEmitStateID ++) {
        Dtw_Column_Node & dtw_node = link[preIdx].nodes[noEmitStateID];

        // dont forward from non-value state
        if(dtw_node.lastUpdate < columnIdx)
            continue;

        double preCost = dtw_node.cost;
        double nextCost;

        // preStateID --- nonEmitEdges[eID] ---> nextStateID
        for(int eID = states[noEmitStateID].noEmitHead; eID != NIL_EDGE; eID = nonEmitEdges[eID].next) {
            nextStateID = nonEmitEdges[eID].to;

            // penalty
            nextCost = preCost + nonEmitEdges[eID].cost;

            tryUpdate(link, columnIdx, noEmitStateID, nextStateID, nextCost);
        }
    }

    return bestVal;
}

void SeqModel::tryUpdate(Dtw_Column_Link * link, int columnIdx, int preStateID, int stateID, double newCost) {
    int rollIdx = getRollIdx(columnIdx);
    int preIdx = rollIdx ^ 1;

    Dtw_Column_Node & dtw_node = link[rollIdx].nodes[stateID];

//    printf("ddx %d %d %lf\n", preStateID, stateID, newCost);

    /*
        if(!(newCost>= 0) && !(newCost<= 0)) {
            printf("%lf\n", newCost);
            printf("%d %d\n", preStateID, stateID);
                getchar();
        }
        */

    bool updateFlag = false;

    // new node!!
    if(dtw_node.lastUpdate != columnIdx) {
        link[rollIdx].addColumnNode( stateID );

        dtw_node.lastUpdate = columnIdx;

        // new nodes' backPtr should be inited to -1
        // This complex logic.. is a tradeoff for high efficiency beam search
        dtw_node.preIdx = NIL_BACK_PTR;

        updateFlag = true;
    }
    else if(Feature::better(newCost, dtw_node.cost)) 
        updateFlag = true;

    if(updateFlag) {
//        printf("new \n");
//        if(newCost > Feature::IllegalDist) 
        dtw_node.cost = newCost;

        // record path 
        if(path_type == FULL_PATH) {
            // bug 如果从一个nonemit 过来的， 则path指向nonemit指向的单词就可以了
            // 虽然nonemit到nonemit是同一个列之间的forward，但编码时候是从前一列
            // forward过来的，这样可以避免用新的值更新
            //
//            while(preStateID >= 0 && ! isEmit(preStateID) )
//                preStateID = link[preIdx].nodes[preStateID].preIdx;

            fullPath[columnIdx][stateID] = preStateID;

            dtw_node.preIdx = preStateID;
        }

        else if(path_type == BACK_PTR) {
            if(isEmit(preStateID) && isEmit(stateID)) {
                dtw_node.preIdx = link[preIdx].nodes[preStateID].preIdx;
            }
            else if(!isEmit(preStateID) && !isEmit(stateID) ) {
//                printf("%d %d\n", preStateID, stateID);
                // preIdx !!  it is a trans inner a t, however i copy old value to preIdx.
                dtw_node.preIdx = link[preIdx].nodes[preStateID].preIdx;
            }
            else if(!isEmit(preStateID) && isEmit(stateID) ) {
                dtw_node.preIdx = link[preIdx].nodes[preStateID].preIdx;
            }
            else if(isEmit(preStateID) && ! isEmit(stateID) ) {
                // new back ptr
                // should be forwarding

                // same column, same rollIdx , forwarding

                if(dtw_node.preIdx == NIL_BACK_PTR) {
                    backPtrs.push_back( BackPtr() );
                    dtw_node.preIdx = backPtrs.size() - 1;
                }

                backPtrs[dtw_node.preIdx].word = states[preStateID].word;

                backPtrs[dtw_node.preIdx].prePtr = link[rollIdx].nodes[preStateID].preIdx; //dtw_node.preIdx;
                backPtrs[dtw_node.preIdx].stateID = stateID;
            }
        }

        /*  
        if(!(newCost>= 0) && !(newCost<= 0)) {
            printf("Star %lf %lf %lf\n", 1.0, states[stateID].forwardCost,  newCost);
            getchar();
        }
        */

        tryForwardToStar( link, columnIdx, stateID, newCost );
    }
}

void SeqModel::tryForwardToStar( Dtw_Column_Link *link, int columnIdx, int stateID, double preCost) {
    if( states[stateID].leafForwardIdx == NIL_FORWARD ) 
        return ;

    double newCost = preCost + states[stateID].forwardCost;

//    if(states[stateID].leafForwardIdx == 1)
//        printf("xxd %lf\n", newCost);

    tryUpdate(link, columnIdx, stateID, states[stateID].leafForwardIdx, newCost);

    /*  
        if(!(newCost>= 0) && !(newCost<= 0)) {
            printf("Star %lf %lf %lf\n", preCost, states[stateID].forwardCost,  newCost);
                getchar();
        }
        */

}
