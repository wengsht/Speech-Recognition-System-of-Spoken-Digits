// =====================================================================================
// 
//       Filename:  parseParseGraph.h
// 
//    Description:  Parse graph
// 
//        Version:  0.01
//        Created:  2014/11/18 20时20分05秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_ParseGraph_H_
#define _AUTOGUARD_ParseGraph_H_

#include <string>
#include <vector>

#define EDGE "Edge"

struct GraphEdge {
    int from, to;
    std::string word;
    double probability;
};

class ParseGraph {
    public:
        ParseGraph() {}
        ~ParseGraph() {}

        void parseGraph(const char * filename);

        // parse from "1+2+6+5" 
        void parseSeqStr(const std::string & seqStr);

        void dump(std::ostream & out) const;
        
        int size() const {
            return edges.size();
        }
        const GraphEdge & operator [] (int idx) const {
            return edges[idx];
        }
#define GRAPH_PARAS(type, para) \
        type get ## para() const { \
            return para; \
        }
#include "graphParas.def"
#undef GRAPH_PARAS

    private:
#define GRAPH_PARAS(type, para) \
        type para;
#include "graphParas.def"
#undef GRAPH_PARAS

        std::vector< GraphEdge > edges;

};

#endif

