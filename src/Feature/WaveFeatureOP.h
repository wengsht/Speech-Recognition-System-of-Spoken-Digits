// =====================================================================================
// 
//       Filename:  WaveFeatureOP.h
// 
//    Description:  This CLASS store one template feature, and apply dtw on inputed data feature with the template 
//
//    This class also be used to store data feature when there are many inputed feature to computed, it is convenient to buffer them 
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
private:
    // -1, only store the edge link list 
    // use (-1 - BestRowNum) to seperate the column
    const static int PathSplitIdx;
    // 边表 (-1 -  最优值行) | 边表 (-1 -  最优值行) |  边表 (-1 -  最优值行) ...
    std::vector<int> path;
    //
//    READ_WRITE_DECLARE(std::vector <int>, path, Path);

    // Template feature, one object, one template
    REFERENCE_READ_ONLY_DECLARE(std::vector<Feature>, templateFeature, TemplateFeature);


    // doRecordPath = true for store the path 
    READ_WRITE_DECLARE(bool, doRecordPath, DoRecordPath);

    // store the filename(完整路径) of this feature
    READ_WRITE_DECLARE(std::string, templateFileName, FileName);

    // store the word of this feature
    READ_WRITE_DECLARE(std::string, _word, Word);

    // Raw / Beam
    READ_WRITE_DECLARE(WaveFeatureOP::OpType, opType, OpType);

    // Store the bestValue
    // When Syn Dtw, It buffer the bestValue on the current column
    READ_ONLY_DECLARE(double, bestValue, DtwResult);

public:
    WaveFeatureOP(const std::vector<Feature> &templateFeature, std::string fileName = "null", std::string word = "null");
    ~WaveFeatureOP() {}

    // Before  Syn DTW, you have to past the input feature's pointer into this template, you should ensure the pointer is not be freed during the dtw process
    void synInit(std::vector<Feature> * inputFeature);

    // return best value every time
    double forwardColumn(double threshold = 0.0);

    // asynchronization, return distance
    double asynDtw(std::vector<Feature> *, double addThreshold = -5.0);

    // length of template
    int getRowNum();

    // 模拟vector， 访问底层数据
    Feature & operator[](int idx);
    int size();

    // dump the color path into an ostream
    SP_RESULT dumpColorPath(std::ostream & Out);

    // 根据前缀名判断是不是同一个单词
    bool operator == (const WaveFeatureOP & b);

private:
    // 边表
    int head[2];
    std::vector<int> columnNxt[2];
    // 当前列值， double
    std::vector<double> columnDtwRes[2];
    // 最后计算了某一行值的idx, 可以省掉memset的O(n)操作
    std::vector<int> lastUpdate;
    
    std::vector<Feature>* inputFeature;
    // Syn Dtw, each forwardColumn() operation will incrase columnIdx by 1
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
        if(lastUpdate[rowIdx] != columnIdx || Feature::better(newPathDist, columnDtwRes[currentRollIdx][rowIdx])) 
            columnDtwRes[currentRollIdx][rowIdx] = newPathDist;

        // 如果没有加入到链表过， 加入
        if(lastUpdate[rowIdx] != columnIdx) {
            lastUpdate[rowIdx] = columnIdx;

            columnNxt[currentRollIdx][rowIdx] = head[currentRollIdx];
            head[currentRollIdx] = rowIdx;
        }
    }
    // 滚动数组的下标
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
