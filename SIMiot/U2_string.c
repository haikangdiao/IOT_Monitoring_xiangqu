#include"uart2.h"
//#include<str.h>

int U2_strSend(char* pc_str,int i_len){
    //unsigned int ui_count = 0;
    //while('\0' != *pc_str && --i_len){
    while(i_len--){
        U2_Send(pc_str);
        //++ui_count;
        ++pc_str;
    }
    return i_len;
}

int U2_strRec(char* pc_str,int i_len){
    int i_count = 0;
    for(;i_count < i_len;++i_count){
        pc_str[i_count] = U2_Rec();
        //if('\0' == pc_str[ui_count]){
        //    break;
        //}
    }
    return i_count;
}