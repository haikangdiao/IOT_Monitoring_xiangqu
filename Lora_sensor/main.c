#include<str.h>
#include"U1_string.h"
#include"U2_string.h"
#include"sensor.h"
#include"Lora.h"

#define sensor_len 50

void user_interrupt(){}

// char str[10] = "0123456789";

// char c = '3';

int main(){
    U1_strInit();
    U2_strInit();
    // MemoryWrite32(U1_CTL0_REG, 0x10);
    // char str[10] = "0123456789";
    // for(int i = 0;i < 10;++i){
    //     putch(str[i]);
    // }
    // c = '3';
    // putch(c);
    char buff[90] = "";
    char c_count;
    int flag;
    while(1){
        getch();
        U2_strSend("1234",4);
        // flag = get_sensor(buff,sensor_len);

        // if(!flag){
        //     for(int i = 0;i < sensor_len;++i){
        //         buff[i] = 0xFF;
        //     }
        // }


        // zig_send(buff,sensor_len);

        
        // for(int i = 0;i < 1000;++i){
        //     delayms();
        // }

        // c_count = getch();

        // putch('1');

        // delayms();

        // for(int i = 0;i < c_count;++i){
        //     delayms();
        // }

        // putch('2');

        // get_sensor(buff,sensor_len);

        // zig_send(buff,sensor_len);

        //get_sensor(buff,sensor_len);

        // U1_Rec();

        // int i = U1_strRec(buff,10);

        // putch(U1_flag_time_over + 0x30);
        // putch(i + 0x30);

    }
    return 0;
}