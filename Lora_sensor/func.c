#include "func.h"

//延时
void delayms(){
    for(int i = 0;i < 0xF8;++i){
        for(int j = 0;j < 0x05;++j){}
    }
}

char* strcomb(char* pc_ori,char* pc_add,int i_alen){
    for(int i = 0;i < i_alen;++i){
        pc_ori[i] = pc_add[i];
    }
    return pc_ori;
}