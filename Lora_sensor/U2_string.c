#include"U2_string.h"

void U2_strInit(){
    U2_Init();
}

int U2_strSend(char* pc_str,int i_len){
    while(i_len--){
        U2_Send(pc_str);
        if(U2_flag_time_over){
            break;
        }
        ++pc_str;
    }
    return i_len;
}

int U2_strRec(char* pc_str,int i_len){
    int i_count = 0;
    for(;i_count < i_len;++i_count){
        pc_str[i_count] = U2_Rec();
        if(U2_flag_time_over){
            break;
        }
    }
    return i_count;
}