/*--------------------------------------------------------------------
 * TITLE: main function for sensor devices
 * AUTHOR: haikang
 * DATE CREATED: 2019/7/19
 * FILENAME: main.c
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/

#include<str.h>
#include"uart2.h"
#include"sensor.h"
#include"Lora.h"
#include"func.h"
#include"uart1.h"
#include <mcu.h>

#define Sensor_Back_data_len            50
#define Device_Lora_adress_high         0X20
#define Device_Lora_adress_low          0X02
#define Gateway_Lora_adress_high        0x20
#define Gateway_Lora_adress_low         0x01
#define Upload_message_len              28

char Uart2_interrupt_flag[1] = {0x0};                   //flag that whether there is message download from gateway
char Lora_Downward_message[10];                         //message from the gateway

void user_interrupt(){
    int i_flag = 0;
    i_flag=Lora_Data_Receive(Lora_Downward_message,6);  //receive the message
    if(i_flag == 1)                                     //verify that the message is received correctly
        *Uart2_interrupt_flag = 0x1;
    else
        *Uart2_interrupt_flag = 0x2;                    //0x1 means correct, 0x2 means wrong
    MemoryWrite32(U2_CLRIRQ_REG,0x01);                  //clear the interrupt flag
}

int main(){
    U1_Init(); 
    U2_Init();
    char Sensor_Back_data[50];
    char Sensor_Upload_message[30]={0xFE,0xFE,Device_Lora_adress_high,Device_Lora_adress_low};
    int i,flag;

    puts("test1 begin!\n");
    MemoryWrite32(U2_CTL0_REG, 0x11);                   //UART2 interrupt enable
    MemoryWrite32(0x1f800700,  0x01);                   //systerm interrupt enable
    for (i = 0; i < 2000000;i++){}                      //delay a little time
    get_sensor_data(Sensor_Back_data,Sensor_Back_data_len);            
    proc_data(Sensor_Back_data, 50, Sensor_Upload_message);         
    while(1){
        if(Uart2_interrupt_flag[0] == 0x1){
            if((Lora_Downward_message[2]==0xDD) && (Lora_Downward_message[3]==0xDD)){   //means that device need upload sensor message
                
                get_sensor_data(Sensor_Back_data,Sensor_Back_data_len);
                flag=proc_data(Sensor_Back_data, 50, Sensor_Upload_message);
                while(flag){                                                            //verify that the message is correct
                    get_sensor_data(Sensor_Back_data,Sensor_Back_data_len);             //repeatedly get sensor data if the message is wrong
                    flag=proc_data(Sensor_Back_data, 50, Sensor_Upload_message);
                    puts("get sensor data error\n");
                }
                Lora_Data_Send(Sensor_Upload_message, Upload_message_len, Gateway_Lora_adress_high, Gateway_Lora_adress_low);
                puts("send message successfully!\n");
            }
            else{
                switch (Lora_Downward_message[3])                                       //other message meaning 
                {
                case 0x00:                                                              //gateway have received the correct message
                    puts("message send successfully!\n");
                    break;
                case 0x01:                                                              //gateway have received the wrong message
                    puts("The total length of the data is incorrect!\n");
                    break;
                case 0x02:
                    puts("Error in verification result!\n");
                    break;
                case 0x03:
                    puts("No start byte or end byte!\n");
                    break;
                case 0x04:
                    puts("Data wait timeout!\n");
                    break;
                default:
                    puts("another error!!!\n");
                    break;
                }
                Lora_Data_Send(Sensor_Upload_message, Upload_message_len, Gateway_Lora_adress_high, Gateway_Lora_adress_low);
            }
            Uart2_interrupt_flag[0] = 0;                                                
            MemoryWrite32(U2_CTL0_REG, 0x11);                                          //reopen UART2 interrupt enable bit
        }
        
    }
        
    return 0;
}