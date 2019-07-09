#include <mcu.h>
#include "uart1.h"

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
    return 0xFF;
}