// =====================================================================================
// 
//       Filename:  WaveFeatureOP.cpp
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/19 16时07分00秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================

#include "WaveFeatureOP.h"
#include <iostream>

const int WaveFeatureOP::PathSplitIdx = -1;

WaveFeatureOP::WaveFeatureOP(const std::vector<Feature> &templateFeature, std::string fileName, std::string word) : inputFeature(NULL), \
    doRecordPath(false), \
    columnIdx(-1), \
    opType(Raw), \
    templateFeature(templateFeature),
    templateFileName(fileName),
    _word(word) {
}

// 这一步要初始化滚动链表
// 实际上asyn dtw 也是调用这一个init， 只不过forward的时候是串行的
void WaveFeatureOP::synInit(std::vector<Feature> *inputFeature) {
    // 初始化第-1列， 滚动链表还是很方便的
    int currentRollIdx = getRollIdx(-1);
    int &idx = currentRollIdx;

    int rowSiz = getRowNum();
    head[idx ^ 1] = -1;
    head[idx] = -1;
    columnNxt[idx ^ 1].resize(rowSiz);
    columnNxt[idx].resize(rowSiz);
    columnDtwRes[idx ^ 1].resize(rowSiz);
    columnDtwRes[idx].resize(rowSiz);

    lastUpdate.resize(rowSiz);
    int i;
    for(i = 0; i < rowSiz; i++)
        lastUpdate[i] = -1;

    this->inputFeature = inputFeature;

    // 初始化第-1列
    columnDtwRes[idx][0] = 0.0;
    head[idx] = 0;

    columnNxt[idx][0] = -1;

    columnIdx = 0;

    path.clear();
}

// 此处传的threashold 是 BestValue + add_threshold
// 因为BestValue是全局的概念，所以在这个类里只能靠外部支持
double WaveFeatureOP::forwardColumn(double threshold) {
    bestValue = Feature::IllegalDist;
    int bestIdx = 0;
    int rowSiz = getRowNum();
    int columnSiz = inputFeature->size();
    if(columnIdx < 0 || columnIdx >= columnSiz) {
        Warn("forward may not init or just finish\n");
        return 0.0;
    }

    int currentRollIdx = getRollIdx(columnIdx);

    int preIdx = currentRollIdx ^ 1;

    // 清空边表
    head[currentRollIdx] = -1;

    int preRowIdx = 0;
    for(preRowIdx = head[preIdx]; preRowIdx != -1; preRowIdx = columnNxt[preIdx][preRowIdx]) {
        // Beam 
        if(opType == Beam) {
            if(columnDtwRes[preIdx][preRowIdx] < threshold) 
                continue;
        }

        double pathValue = columnDtwRes[preIdx][preRowIdx];

        // 对应三种扩展
        int addIdx;
        for(addIdx = 0; addIdx < 3; addIdx ++) {
            int nxtRowIdx = addIdx + preRowIdx;

            if(nxtRowIdx >= rowSiz)
                break;

            double newPathValue = pathValue + calDist(nxtRowIdx, columnIdx);

            updateDtwNode(columnIdx, nxtRowIdx, newPathValue);

            if(Feature::better(newPathValue, bestValue)) {
//            if(bestValue == Feature::IllegalDist || bestValue < newPathValue) {
                bestValue = newPathValue;
                bestIdx   = nxtRowIdx;
            }
        }
    }

    if(doRecordPath) {
        recordPath(bestIdx);
    }

    columnIdx ++;

    return bestValue;
}

int WaveFeatureOP::getRowNum() {
    return templateFeature.size();
}
double WaveFeatureOP::asynDtw(std::vector<Feature> * inputFeature, double addThreshold) {
    synInit(inputFeature);

    int idx;

    int columnSiz = inputFeature->size();

    double res = 0.0;
    for(idx = 0; idx < columnSiz; idx++) {
        res = forwardColumn(res + addThreshold);
    }

    return res;
}

SP_RESULT WaveFeatureOP::dumpColorPath(std::ostream & Out) {
    if(!doRecordPath) 
        return SP_PATH_NOT_EXIT;

    Out << "Template File Name: " << templateFileName << std::endl;

    Out << "Dtw Result: " ; //
    if(getDtwResult() == Feature::IllegalDist)
        Out << "Illedge Value: " ; //
    else 
        Out << getDtwResult(); 
    Out << std::endl;

    int rowNum = getRowNum();
    std::vector< std::vector<int> > pathImg(rowNum);
    int idx = 0, siz = path.size(), column = 0;
    int finalIdx;

    while(idx < siz) {
        if(pathImg[0].size() <= column) {
            for(int j = 0;j < rowNum;j++) 
                pathImg[j].push_back(0);
        }

        if(path[idx] < 0) {
            int bestIdx = WaveFeatureOP::PathSplitIdx - path[idx];
            if(bestIdx >= 0 && bestIdx < rowNum)
                pathImg[rowNum - bestIdx - 1][column] = 2;

            column ++;
            idx ++;
            continue;
        }
        pathImg[rowNum - path[idx] - 1][column] = 1;
        idx ++;
    }

    for(int i = 0;i < rowNum;i++) {
        for(int j = 0;j < pathImg[i].size(); j++) {
            if(2 == pathImg[i][j]) Out << BLUE_BACK;
            else if(1 == pathImg[i][j]) Out << GREEN_BACK;
            else  Out << RED_BACK ;
            Out << " ";
        }
        Out << std::endl;
    }
    Out << NONE_BACK << std::endl;
    return SP_SUCCESS; 
}

bool WaveFeatureOP::operator==(const WaveFeatureOP & b) {
    return _word == b.getWord();
}
