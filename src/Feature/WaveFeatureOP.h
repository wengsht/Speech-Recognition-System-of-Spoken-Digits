// =====================================================================================
// 
//       Filename:  WaveFeatureOP.h
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/10/19 15时54分38秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_WaveFeature_H_
#define _AUTOGUARD_WaveFeature_H_

#include <iostream>
#include <vector>
#include "tool.h"
#include "Feature.h"
#include "configure_basic.h"
#include <string>

class WaveFeatureOP {
public:
    enum OpType {
        Raw,
        Beam
    };
    const static int PathSplitIdx;
private:

    READ_WRITE_DECLARE(std::vector<Feature>, templateFeature, TemplateFeature);

    // 边表 (-1 -  最优值列) | 边表 (-1 -  最优值列) |  边表 (-1 -  最优值列) ...
    //
    READ_WRITE_DECLARE(std::vector <int>, path, Path);

    READ_WRITE_DECLARE(bool, doRecordPath, DoRecordPath);

    READ_WRITE_DECLARE(std::string, templateFileName, FileName);

    READ_WRITE_DECLARE(WaveFeatureOP::OpType, opType, OpType);

    READ_ONLY_DECLARE(double, bestValue, DtwResult);
public:
    WaveFeatureOP(const std::vector<Feature> &templateFeature, std::string fileName = "null");
    ~WaveFeatureOP() {}

    void synInit(std::vector<Feature> * inputFeature);

    // return best value every time
    double forwardColumn(double threshold = 0.0);

    // asynchronization, return distance
    double asynDtw(std::vector<Feature> *, double addThreshold = -5.0);

    // length of template
    int getRowNum();

    SP_RESULT dumpColorPath(std::ostream & Out);

private:
    // 边表
    std::vector<int> columnNxt[2];
    // 当前列值， double
    std::vector<double> columnDtwRes[2];
    // 最后计算了某一行值的idx， 防止边表存储两个实例
    std::vector<int> lastUpdate;
    int head[2];
    
    std::vector<Feature>* inputFeature;
    int columnIdx;

private:
    // 计算 template 第rowidx个节点和data第columnidx个节点的距离
    // TODO tmp calculated values!
    double calDist(int rowIdx, int columnIdx) {
        return templateFeature[rowIdx] - (*inputFeature)[columnIdx];
    }
    // 更新一次dp操作
    void updateDtwNode(int columnIdx, int rowIdx, double newPathDist) {
        int currentRollIdx = getRollIdx(columnIdx);
        if(lastUpdate[rowIdx] != columnIdx || columnDtwRes[currentRollIdx][rowIdx] < newPathDist) 
            columnDtwRes[currentRollIdx][rowIdx] = newPathDist;

        // 如果没有加入到链表过， 加入
        if(lastUpdate[rowIdx] != columnIdx) {
            lastUpdate[rowIdx] = columnIdx;

            columnNxt[currentRollIdx][rowIdx] = head[currentRollIdx];
            head[currentRollIdx] = rowIdx;
        }
    }
    inline int getRollIdx(int columnIdx) {
        return columnIdx & 1;
    }
    void recordPath(int bestIdx) {
        // the first idx save final idx
        
        int currentRollIdx = getRollIdx(columnIdx);

        int rowIdx;
        int cnt = 0;
        for(rowIdx = head[currentRollIdx]; rowIdx != -1; rowIdx = columnNxt[currentRollIdx][rowIdx]) {
            cnt ++;
            path.push_back(rowIdx);
        }
        if(cnt == 0) bestIdx = getRowNum();
        path.push_back(PathSplitIdx - bestIdx);
    }
};

#endif