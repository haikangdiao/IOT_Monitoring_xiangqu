#include <mcu.h>
#include "uart2.h"

//extern unsigned char U2_flag_time_over = 0;

//init uart2
void U2_Init(){
    MemoryWrite32(U2_CTL0_REG, 0x10);
}

//read 8bits from uart2
char U2_Rec(){
    U2_flag_time_over = 0;
    char c_ch = '\0';

    //wait to read uart2
    unsigned int ui_count = U2_Re_Time_out;
    while(!(MemoryRead32(U2_DATA_RDY_REG) & 0X01) && --ui_count){}

    //uart don't have data
    if(!ui_count){
        U2_flag_time_over = 1;
        return '\0';
    }

    //read uart2
    c_ch = MemoryRead32(U2_READ_REG);
    return c_ch;
}

//write 8bits to uart2
char U2_Send(char* pc_ch){
    U2_flag_time_over = 0;

    //wait to send ch to uart2
    unsigned int ui_count = U2_Re_Time_out;
    while((MemoryRead32(U2_BUSY_REG) & 0x01) && --ui_count){}

    if(!ui_count){
        U2_flag_time_over = 1;
        return '\0';
    }

    MemoryWrite32(U2_WRITE_REG,*pc_ch);
    return *pc_ch;
}