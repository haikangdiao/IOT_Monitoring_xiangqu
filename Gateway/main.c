/*--------------------------------------------------------------------
 * TITLE: main function for Gateway 
 * AUTHOR: haikang
 * DATE CREATED: 2019/7/19
 * 
 * FILENAME: main.c
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/

#include<str.h>
#include"uart2.h"
#include"Lora.h"
#include"func.h"
#include"uart1.h"
#include <mcu.h>

#define Device_Lora_adress_high         0X20
#define Device_Lora_adress_low          0X02
#define Gateway_Lora_adress_high        0x20
#define Gateway_Lora_adress_low         0x01


char Uart2_interrupt_flag[1] = {0x0};                   //flag that whether there is message download from gateway
char Lora_Downward_message[10];                         //message from the gateway

void user_interrupt(){
   
}

int main(){
    char Gateway_Lora_command[6][6] = {0xFE, 0XFE, 0X00, 0X00, 0XEF, 0XEF,
                                       0xFE, 0XFE, 0X00, 0X01, 0XEF, 0XEF,
                                       0xFE, 0XFE, 0X00, 0X02, 0XEF, 0XEF,
                                       0xFE, 0XFE, 0X00, 0X03, 0XEF, 0XEF,
                                       0xFE, 0XFE, 0X00, 0X04, 0XEF, 0XEF,
                                       0XFE, 0XFE, 0XDD, 0XDD, 0XEF, 0XEF};
    char Gateway_Lora_sensor_message[30];
    int i, flag = 0;
    int check_flag = 0;
    int Sensor_message_len = 28;
    U1_Init();
    U2_Init();
    char test_s[5] = {"AT\r\n"};
    puts("test1 begin!\r\n");
    while(1){
        for (i = 0; i < 200000; i++){}
        U2_SendInt(0x41);
       
        U2_SendInt(0x54);
        
        U2_SendInt(0x0D); 
        U2_SendInt(0x0A);
        puts("12345");
    }
    
    MemoryWrite32(U2_CTL0_REG, 0x11);                    //UART2 interrupt enable
    MemoryWrite32(0x1f800700, 0x01);                     //systerm interrupt enable
    for (i = 0; i < 200000; i++){}                      //delay a little time
    
    while(1){
        for (i = 0; i < 2000000; i++){}
        Lora_Data_Send(Gateway_Lora_command[6], 6, 0x20, 0x01);
        flag=Lora_Data_Receive(Gateway_Lora_sensor_message, Sensor_message_len);
        check_flag=Lora_message_check(Gateway_Lora_sensor_message, flag, Sensor_message_len);
        Lora_Data_Send(Gateway_Lora_command[check_flag], 6, 0x20, 0x01);
        check_flag = 0;

    }
        
    return 0;
}