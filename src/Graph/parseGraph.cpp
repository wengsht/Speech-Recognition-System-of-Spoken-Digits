// =====================================================================================
// 
//       Filename:  parseParseGraph.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/18 20时29分41秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "parseGraph.h"
#include <fstream>
#include <sstream>
#include "SerialFiles.h"
#include <string>

void ParseGraph::parseSeqStr(const std::string & seqStr) {
    edges.clear();

    // TODO
}

void ParseGraph::parseGraph(const char * filename) {
    edges.clear();

    std::ifstream graphIN;
    graphIN.open(filename, std::ios::in);

    if(graphIN.fail() == true) 
        return ;

    std::string line;
    std::string tmp;

    while(std::getline(graphIN, line)) {
        std::stringstream sin(line);
        if(!(sin >> tmp)) continue;

        // 解析各个参数
#define GRAPH_PARAS(type, para) \
        if(#para ":" == tmp) {\
            sin >> para; \
            continue; \
        }
#include "graphParas.def"
#undef GRAPH_PARAS

        if(EDGE == tmp) {
            GraphEdge newEdge;

            sin >> newEdge.from >> newEdge.to;

            std::getline(sin, tmp, '\"');

            std::getline(sin, tmp, '\"');
            newEdge.word = std::string(SerialFiles::inAlias(const_cast<char *>(tmp.c_str())));

            sin >> newEdge.probability;

            edges.push_back(newEdge);
        }
    }
}

void ParseGraph::dump(std::ostream & out) const {
    for(int i = 0;i < size(); i++) {
        out << edges[i].from << " " << edges[i].to << " " << edges[i].word << " " << edges[i].probability << std::endl;
    }
}
