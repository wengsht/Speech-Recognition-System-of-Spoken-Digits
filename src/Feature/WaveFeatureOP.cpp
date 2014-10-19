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

const int WaveFeatureOP::PathSplitIdx = -1;
WaveFeatureOP::WaveFeatureOP(const std::vector<Feature> &templateFeature) : inputFeature(NULL), doRecordPath(false), columnIdx(-1), opType(Raw),templateFeature(templateFeature) {
}

void WaveFeatureOP::synInit(std::vector<Feature> *inputFeature) {
    int currentRollIdx = getRollIdx(-1);
    int &idx = currentRollIdx;

    int rowSiz = templateFeature.size();
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

double WaveFeatureOP::forwardColumn(double threshold) {
    double bestValue = Feature::IllegalDist;
    int rowSiz = templateFeature.size();
    if(columnIdx < 0 || columnIdx >= rowSiz) {
        Warn("forward may not init or just finish\n");
        return 0.0;
    }

    int currentRollIdx = getRollIdx(columnIdx);

    int preIdx = currentRollIdx ^ 1;

    // 清空边表
    head[currentRollIdx] = -1;

    int preRowIdx = 0;
    for(preRowIdx = head[preIdx]; preRowIdx != -1; preRowIdx = columnNxt[preIdx][preRowIdx]) {
        if(opType == Beam) {
            if(columnDtwRes[preIdx][preRowIdx] > threshold) 
                continue;
        }

        double pathValue = columnDtwRes[preIdx][preRowIdx];

        // 对应三种扩展
        int addIdx;
        for(addIdx = 0; addIdx < 3; addIdx ++) {
            int nxtpreRowIdx = addIdx + preRowIdx;

            if(nxtpreRowIdx >= rowSiz)
                break;

            double newPathValue = pathValue + calDist(nxtpreRowIdx, columnIdx);

            updateDtwNode(columnIdx, nxtpreRowIdx, newPathValue);

            if(bestValue == Feature::IllegalDist || bestValue < newPathValue)
                bestValue = newPathValue;
        }
    }

    if(doRecordPath)
        recordPath();

    columnIdx ++;

    return bestValue;
}

int WaveFeatureOP::getRowNum() {
    return templateFeature.size();
}
double WaveFeatureOP::asynDtw(std::vector<Feature> * inputFeature) {
    synInit(inputFeature);

    opType = Raw;

    int idx;

    int columnSiz = inputFeature->size();

    double res = 0.0;
    for(idx = 0; idx < columnSiz; idx++) {
        res = forwardColumn();
    }

    return res;
}
