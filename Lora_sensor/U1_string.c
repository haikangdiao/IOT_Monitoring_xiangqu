#include"U1_string.h"

void U1_strInit(){
    U1_Init();
}

int U1_strSend(char* pc_str,int i_len){
    while(i_len--){
        U1_Send(pc_str);
        if(U1_flag_time_over){
            break;
        }
        ++pc_str;
    }
    return i_len;
}

int U1_strRec(char* pc_str,int i_len){
    int i_count = 0;
    for(;i_count < i_len;++i_count){
        pc_str[i_count] = U1_Rec();
        if(U1_flag_time_over){
            break;
        }
    }
    return i_count;
}