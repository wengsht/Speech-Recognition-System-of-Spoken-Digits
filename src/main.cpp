

#include "resource.h"

#include "srs.h"

#include <stdio.h>



// 
void solution(){
    BEPAnalysis b_ep;
    AEPAnalysis a_ep;
    DAEPAnalysis da_ep;
    char wav_file_name[64] = "da_ep";
    bool is_capture = true;
    char ch = '\0';
    while(ch != 'L' && ch != 'l' && ch!='C' && ch!='c'){
        Tip("Capture or load from file? input C or L: ");
        ch = getche();
        puts("");
        if(ch=='C' || ch == 'c'){
            is_capture = true;
        }
        else {
            is_capture = false;
        }
    }
    
    Tip("Input a file_name: ");
    scanf("%s%c",wav_file_name,&ch);
    fflush(stdin);
    
    if(is_capture)capture(wav_file_name,"da_ep",da_ep,true);
    else load_calc(wav_file_name,"da_ep",da_ep,false);
    
    load_calc(wav_file_name,"a_ep",a_ep,false);
    load_calc(wav_file_name,"b_ep",b_ep,false);
    Tip("Over");
}

int main()
{
    solution();
    return 0;
}
