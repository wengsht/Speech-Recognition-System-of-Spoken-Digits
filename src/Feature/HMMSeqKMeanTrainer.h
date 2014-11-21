// =====================================================================================
// 
//       Filename:  HMMSeqKmeanTrainer.h
// 
//    Description:  
// 
//        Version:  0.01
//        Created:  2014/11/19 08时45分06秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_HMMSeqKMeanTrainer_H_
#define _AUTOGUARD_HMMSeqKMeanTrainer_H_

#include "HMMSeqTrainer.h"
class HMMSeqKMeanTrainer : public HMMSeqTrainer {
    public:
        HMMSeqKMeanTrainer();
        ~HMMSeqKMeanTrainer();

    private:
        void hmmSeqTrain();

        void initHMMTrain();

        // return true to stop iterate
        bool iteratorSeqTrain();
};

#endif

