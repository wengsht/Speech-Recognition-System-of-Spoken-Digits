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
#include "HMMKMeanAutomaton.h"
#include <pthread.h>
#include <vector>

class HMMSeqKMeanTrainer : public HMMSeqTrainer {
    public:
        HMMSeqKMeanTrainer();
        ~HMMSeqKMeanTrainer();

    private:
        struct segTaskInfo {
            int threadID;
            int threadNum;
            std::vector<SeqWav> *trainWavs;
        };
        struct updateTaskInfo {
            bool *bigChange;
            HMMKMeanAutomaton * automaton;
        };

        static int addressTag(void * address, int range);

        void hmmSeqTrain();

        void initHMMTrain();

        static void segmentTask( void * in);
        static void updateParaTask( void * in);
        static void initParaTask( void * in);

        // return true to stop iterate
        bool iteratorSeqTrain();

        static pthread_mutex_t lock;
};

#endif

