#ifndef __FUNC_H__
#define __FUNC_H__

void delay();

/***************************
 * @brief
 * This function is user_implemented strlen
 * @param     pc_str       the array 
 * @return                 length of the pc_str
 ****************************/
int strlen_u(char* pc_str);

/***************************
 * @brief
 * This function is user_implemented strcpy
 * @param     pc_ori       the array that needs to be filled by pc_add
 * @param     pc_add       populate the content of pc_add into the buff
 * @return                 pointer of the pc_ori that is filled
 ****************************/
char* strcpy_u(char* pc_ori,char* pc_add);

char* strcomb(char* pc_ori,char* pc_add,int i_alen);

/*****************************
 * @brief
 * This function is user_implemented memset
 * @param      buff       the array that is filled
 * @param      num        populate the num into the buff
 * @param      count      length of the buff
 * @return                pointer of buff
 *****************************/
char* char_memset(char* buff,char num,int count);

#endif