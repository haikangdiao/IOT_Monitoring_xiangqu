#include "func.h"
#include "debug_log.h"

//延时
void delay(){
    for(int i = 0;i < 0xFF;++i){
        for(int j = 0;j < 0xFF;++j){}
    }
}

/***************************
 * @brief
 * This function is user_implemented strlen
 * @param     pc_str       the array 
 * @return                 length of the pc_str
 ****************************/
int strlen_u(char* pc_str){
    // DEBUG_STR(pc_str);
    char* tmp = pc_str;
    while('\0' != (*tmp)){
       ++tmp;
    }
    // DEBUG_INT(tmp - pc_str);
    return (tmp - pc_str);
}

/***************************
 * @brief
 * This function is user_implemented strcpy
 * @param     pc_ori       the array that needs to be filled by pc_add
 * @param     pc_add       populate the content of pc_add into the buff
 * @return                 pointer of the pc_ori that is filled
 ****************************/
char* strcpy_u(char* pc_ori,char* pc_add){
    // DEBUG_STR(pc_add);
    while('\0' != (*pc_add)){
        (*pc_ori) = (*pc_add);
        ++pc_ori;
        ++pc_add;
    }
    return pc_ori;
}

char* strcomb(char* pc_ori,char* pc_add,int i_alen){
    for(int i = 0;i < i_alen;++i){
        pc_ori[i] = pc_add[i];
    }
    return pc_ori;
}

/*****************************
 * @brief
 * This function is user_implemented memset
 * @param      buff       the array that is filled
 * @param      num        populate the num into the buff
 * @param      count      length of the buff
 * @return                pointer of buff
 *****************************/
char* char_memset(char* buff,char num,int count){
    char* len = buff + count; 
    while(buff < len){
        (*buff) = num;
        ++buff;
    }
}