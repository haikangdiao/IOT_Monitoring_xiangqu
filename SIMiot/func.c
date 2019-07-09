#include "func.h"

//延时
void delay(){
    for(int i = 0;i < 0xFF;++i){
        for(int j = 0;j < 0xFF;++j){}
    }
}

char* strcomb(char* pc_ori,char* pc_add,int i_alen){
    for(int i = 0;i < i_alen;++i){
        pc_ori[i] = pc_add[i];
    }
    return pc_ori;
}