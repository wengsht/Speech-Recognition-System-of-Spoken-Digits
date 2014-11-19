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

struct SeqState {
    SeqState() : hmmState(NULL), head(NIL_EDGE), leafForwardIdx(NIL_FORWARD), emitHead(-1), word(NULL) {
    }

    HMMState * hmmState;

    const char * word;

    int head; // state edge table
    int emitHead; //non state edge table
    int leafForwardIdx; // forward to none emitted state
    double forwardCost;
};

struct Dtw_Column_Node {
    Dtw_Column_Node() : lastUpdate(NONE_UPDATE) {

    }

    int preIdx; // used by backPtr search
                // for full path, use a matrix to store
    double cost;
    int lastUpdate;
    int next;
};

// for beam, should be a link
struct Dtw_Column_Link {
    int head;

    Dtw_Column_Node * nodes;
};

struct SeqEdge {
    int to;
    int next;

    double cost; // p2cost
};

struct BackPtr {
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

        void recognition(WaveFeatureOP & input, std::vector<std::string> & res);

        void close();

        void dumpDot(std::ostream &out);
    private:
        void refreshEdge(const GraphEdge & simpleEdge, std::map< std::string, HMMAutomaton *> &automatons, int &edgeCnt, int &stateCnt, int &nonEmitEdgeCnt);

        void refreshEdge(std::vector<SeqEdge> &edges, int from, int to, double cost, int &edgeCnt);

        void refreshEdges( const ParseGraph & graph, std::map< std::string, HMMAutomaton *> & automatons);

        int refreshNode( const HMMAutomaton & automaton, int stateID, int &stateCnt, const std::string & word);

        void initNOEmitStates( const ParseGraph & graph );

        void dtw(WaveFeatureOP & wav, SEQ_DTW_PATH_TYPE path_type);
        void setBeam(double thr);

        int getRollIdx(int columnIdx) {
            return (columnIdx & 1);
        }
    private:
        int N_States;
        int Start_State;
        std::vector<SeqState> states;
        std::vector<SeqEdge>  edges;
        std::vector<SeqEdge>  nonEmitEdges;

        Matrix<int> fullPath;
        std::vector< BackPtr > backptrs;

        // use backPtrs to match
        // match with fullPath or backPtrs
        std::vector< std::string >  matchRes;

        bool doBeam;
        double beamThr;
};

#endif

