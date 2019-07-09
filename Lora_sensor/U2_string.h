#ifndef __U2_STRING_H__
#define __U2_STRING_H__

#include"uart2.h"

void U2_strInit();
int U2_strSend(char* pc_str,int i_len);
int U2_strRec(char* pc_str,int i_len);

#endif