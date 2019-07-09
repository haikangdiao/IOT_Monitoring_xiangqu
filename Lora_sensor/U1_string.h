#ifndef __U1_STRING_H__
#define __U1_STRING_H__

#include"uart1.h"

void U1_strInit();
int U1_strSend(char* pc_str,int i_len);
int U1_strRec(char* pc_str,int i_len);

#endif