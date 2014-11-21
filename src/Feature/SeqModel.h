// =====================================================================================
// 
//       Filename:  SeqModel.h
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/18 22时45分09秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_SeqModel_H_
#define _AUTOGUARD_SeqModel_H_

#include "parseGraph.h"
#include <map>
#include <string>
#include <vector>
#include "HMMAutomaton.h"
#include "HMMState.h"
#include <iostream>

#define NIL_EDGE -1
#define NIL_FORWARD -1
#define NONE_UPDATE -1
#define NIL_BACK_PTR -1

struct SeqState {
    SeqState() : hmmState(NULL), head(NIL_EDGE), leafForwardIdx(NIL_FORWARD), noEmitHead(-1), word(NULL) {
    }

    HMMState * hmmState;

    const char * word;

    int head; // state edge table
    int noEmitHead; //non state edge table
    int leafForwardIdx; // forward to none emitted state
    double forwardCost;
};

struct Dtw_Column_Node {
    Dtw_Column_Node() : lastUpdate(NONE_UPDATE), preIdx(NIL_BACK_PTR) { // , backPtr(NIL_BACK_PTR) {

    }

    int preIdx; // used by backPtr search
                // for full path, use a matrix to store
                // this variable also buffer a ptr to preState, this is used by non-emit forward to non-emit
                
    double cost;
    int lastUpdate;
    int next;

//    int backPtr; // link to this-state's backPtr, for no emit state
};

// for beam, should be a link
struct Dtw_Column_Link {
    int head;

    Dtw_Column_Node * nodes;

    void addColumnNode(int node) {
        nodes[node].next = head;
        head = node;
    }
    void clear() {
        head = NIL_EDGE;
    }
};

struct SeqEdge {
    int to;
    int next;

    double cost; // p2cost
};

struct BackPtr {
    int stateID; // used for check if it is terminal state
    const char * word;
    int prePtr;
};

class SeqModel {
    public:
        enum SEQ_DTW_PATH_TYPE {
            NO_RECORD,
            FULL_PATH,
            BACK_PTR
        };

        SeqModel();
        ~SeqModel();

        void buildModel(const ParseGraph & graph, std::map< std::string, HMMAutomaton *> & automatons);

        void recognition(WaveFeatureOP & input, std::vector<std::string> & res, SEQ_DTW_PATH_TYPE path_type = BACK_PTR);


        void close();

        void dumpDot(std::ostream &out);

        void setBeam(double thr);
    private:
        // build or refresh an edge ( one edge means one hmm model here!!)
        void refreshEdge(const GraphEdge & simpleEdge, std::map< std::string, HMMAutomaton *> &automatons, int &edgeCnt, int &stateCnt, int &nonEmitEdgeCnt);

        // build or refresh an edge( one edge means really edge in the "trie tree", 
        void refreshEdge(std::vector<SeqEdge> &edges, int from, int to, double cost, int &edgeCnt);

        // build or refresh based on a graph, it will call refreshEdge(GraphEdge, ...) on all of the hmm edge 
        void refreshEdges( const ParseGraph & graph, std::map< std::string, HMMAutomaton *> & automatons);

        // build or refresh a state node(emit or non-emit)
        int refreshNode( const HMMAutomaton & automaton, int stateID, int &stateCnt, const std::string & word);

        // init no emit states, [0, N_States-1]
        void initNOEmitStates( const ParseGraph & graph );

        // main process
        void dtw(WaveFeatureOP & wav, SEQ_DTW_PATH_TYPE path_type);

        int getRollIdx(int columnIdx) {
            return (columnIdx & 1);
        }

        void setPathType(SEQ_DTW_PATH_TYPE path_type) {
            this->path_type = path_type;
        }

        // forward a column 
        // it will firstly forward from emit states
        // then forward from un-emit states
        double forwardColumn(Dtw_Column_Link *link, WaveFeatureOP & wav, int columnIdx, double threshold);

        // update a column node 
        void tryUpdate(Dtw_Column_Link * link, int columnIdx, int preStateID, int stateID, double newCost);

        // forward from leaf to star
        // once a leaf was updated, this function will be called
        void tryForwardToStar( Dtw_Column_Link *link, int columnIdx, int stateID, double preCost);

        // called at the beginning of dtw()
        // prepare a full matrix for full_path
        // or init NIL backptr for   BACK_PTR
        void preparePathRecord(WaveFeatureOP &wav);

        // should be called only after in FULL_PATH mode
        void collectBestPath( std::vector<int> &path, int wavSiz );

        // path_type = FULL_PATH -->   dump result from full_path into res
        // path_type = BACK_PTR  -->   dump result from back ptr
        void collectRes(std::vector<std::string> &res, SEQ_DTW_PATH_TYPE path_type, int wavSiz);

        void collectResFromFullPath(std::vector<std::string> &res, int wavSiz);
        void collectResFromBackPtr(std::vector<std::string> &res);


        bool isEmit( int stateID ) {
            return stateID >= N_States;
        }
    private:

        // Check graphParas.def 
#define GRAPH_PARAS(type, para) \
        type para;
#include "graphParas.def"
#undef GRAPH_PARAS


        std::vector<SeqState> states;
        std::vector<SeqEdge>  edges;
        std::vector<SeqEdge>  nonEmitEdges;

        Matrix<int> fullPath;
        std::vector< BackPtr > backPtrs;

        // use backPtrs to match
        // match with fullPath or backPtrs
        std::vector< std::string >  matchRes;

        bool doBeam;
        double beamThr;

        SEQ_DTW_PATH_TYPE path_type;
};

#endif

