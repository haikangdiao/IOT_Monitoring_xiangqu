#include "mcu.h"
#include "uart1.h"
#include "str.h"

//extern unsigned char U1_flag_time_over = 0;

//init uart1
void U1_Init(){
    MemoryWrite32(U1_CTL0_REG, 0x10);
}

//read 8bits from uart1
char U1_Rec(){
    U1_flag_time_over = 0;
    char c_ch = '\0';

    //wait to read uart1
    unsigned int ui_count = U1_Re_Time_out;
    while(!(MemoryRead32(U1_DATA_RDY_REG) & 0X01) && --ui_count){}

    //uart don't have data
    if(!ui_count){
        U1_flag_time_over = 1;
        return '\0';
    }

    //read uart1
    c_ch = MemoryRead32(U1_READ_REG);
    return c_ch;
}

//write 8bits to uart1
char U1_Send(char* pc_ch){
    U1_flag_time_over = 0;

    //wait to send ch to uart1
    unsigned int ui_count = U1_Re_Time_out;
    while((MemoryRead32(U1_BUSY_REG) & 0x01) && --ui_count){}

    if(!ui_count){
        U1_flag_time_over = 1;
        return '\0';
    }

    MemoryWrite32(U1_WRITE_REG,*pc_ch);
    return *pc_ch;
}

void U1_SendInt(int c){
  while(MemoryRead32(U1_BUSY_REG)){}  //checking if Tx is busy
  MemoryWrite32(U1_WRITE_REG,c);      //sending c out
}

unsigned int U1_strSend(char* pc_str,unsigned int i_len){
    while(i_len--){
        U1_Send(pc_str);
        if(U1_flag_time_over){
            break;
        }
        ++pc_str;
    }
    i_len++;
    return i_len;
}

int U1_strRec(char* pc_str,int i_len){
    int i_count = 0;
    for(;i_count < i_len;++i_count){
        pc_str[i_count] = U1_Rec();
        if(U1_flag_time_over){
            puts("no data\n");
            break;
        }
    }
    return i_count;
} 