#include"Lora.h"

int Lora_send(char* pc_str,int i_len){
    return U2_strSend(pc_str,i_len);
}   