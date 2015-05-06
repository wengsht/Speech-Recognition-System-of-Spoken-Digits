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
#include "configure_hmm.h"
#include <iostream>


void ParseGraph::dumpSeqStr2Vec(const std::string & seqStr, std::vector<std::string> & res) {
    char * words = new char[seqStr.length() + 1];

    strcpy(words, seqStr.c_str());

    char *word;

    for(word = strtok(words, LINK_WORD); word; word = strtok(NULL, LINK_WORD)) {
        res.push_back(std::string( SerialFiles::inAlias(word) ));
    }

    delete [] words;
}
void ParseGraph::parseSeqStr(const std::string & seqStr) {
    edges.clear();

    std::vector< std::string > words;
    ParseGraph::dumpSeqStr2Vec(seqStr, words);

//    char * words = new char[seqStr.length() + 1];
//    strcpy(words, seqStr.c_str());

//    char *word;

    int node;
    for(node = 0; node < words.size(); node ++) {
        GraphEdge newEdge;

        newEdge.from = node;
        newEdge.to   = node + 1;
        newEdge.word = words[node];
        newEdge.probability = TRAIN_INNER_PENALTY;

        edges.push_back(newEdge);
    }

    N_States = node + 1;
    Start_State = 0;
    Terminal_States = node;
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
