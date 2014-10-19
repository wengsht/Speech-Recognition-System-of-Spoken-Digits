// =====================================================================================
// 
//       Filename:  SerialFiles.h
// 
//    Description:  Generate serial file names format
//                  e.g. getSerialFileName("haha", 2, out) 
//                  out = haha_0002
//
//        Version:  0.01
//        Created:  2014/10/17 11时52分01秒
//       Revision:  none
//       Compiler:  clang 3.5
// 
//         Author:  wengsht (SYSU-CMU), wengsht.sysu@gmail.com
//        Company:  
// 
// =====================================================================================
#ifndef _AUTOGUARD_SerialFiles_H_
#define _AUTOGUARD_SerialFiles_H_

class SerialFiles {
    public:
        SerialFiles();
        ~SerialFiles();

        void setSizeBit(int sizeBit);

        // out[] = prefix1_prefix2_prefixn_0000seqNum
        void getSerialFileName(char * out, int seqNum, int prefixNum, ...);

        // prefix_0004.wav  --> prefix = "prefix" seqNum = 4
        void parseSerialFileName(char *fileName, int &seqNum, int prefixNum, ...);

    private:
        char *fullfill(int num);

        const static int maxSizeBit;

        // e.g fileName_xxxx.type 
        // sizeBit = 4
        int sizeBit;
};

#endif
