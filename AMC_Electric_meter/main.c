/*--------------------------------------------------------------------
 * TITLE: main function for AMC Electric meter 
 * AUTHOR: haikang
 * DATE CREATED: 2019/8/20
 * 
 * FILENAME: main.c
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/

#include<str.h>
#include<mcu.h>
#include"uart1.h"
#include"uart2.h"
#include"Lora.h"
#include"amc.h"
#include"func.h"


#define AMC_Device_Adress               0X01
#define Device_type                     0x02                                            //sensor's type is 01,and AMC is 02
#define Device_ID                       0x01
#define Device_Lora_adress_high         0X20
#define Device_Lora_adress_low          0X02
#define Gateway_Lora_adress_high        0x20
#define Gateway_Lora_adress_low         0x01

char Uart2_interrupt_flag[1] = {0x0};                                                   //flag that whether there is message download from gateway
char Lora_Downward_message[10];                                                         //message from the gateway

void user_interrupt(){
    int i_flag = 0;
    i_flag=Lora_Data_Receive(Lora_Downward_message,6);                                  //receive the message
    if(i_flag == 1)                                                                     //verify that the message is received correctly
        *Uart2_interrupt_flag = 0x1;
    else
        *Uart2_interrupt_flag = 0x2;                                                    //0x1 means correct, 0x2 means wrong
    MemoryWrite32(U2_CLRIRQ_REG,0x01);                                                  //clear the interrupt flag
}

int main(){
    U1_Init(); 
    U2_Init();
    char AMC_Back_data[25];
    char AMC_Upload_message[30]={0xFE,0xFE,Device_Lora_adress_high,Device_Lora_adress_low,0x00,Device_type,Device_ID};
    int i,flag;
    unsigned int AMC_Register_First_Adress, AMC_Register_Number,Upload_message_len;

    puts("test1 begin!\n");
    MemoryWrite32(U2_CTL0_REG, 0x11);                                                   //UART2 interrupt enable
    MemoryWrite32(0x1f800700,  0x01);                                                   //systerm interrupt enable
    MemoryWrite32(0x1f800017, 0x271);                                                   //Modify the baud rate to 9600
    for (i = 0; i < 2000000;i++){}                                                      //delay a little time

    AMC_Register_First_Adress = 0x11;
    AMC_Register_Number = 0x03;
    Upload_message_len = 8 + 5 + (AMC_Register_Number * 2);                             //get the length of message
    AMC_Read_Configuration(AMC_Device_Adress,AMC_Register_First_Adress,AMC_Register_Number,AMC_Back_data);       
    AMC_data_proc(AMC_Back_data, (5 + AMC_Register_Number*2), AMC_Upload_message);      //get the meter's data


        while (1)
        {
            if (Uart2_interrupt_flag[0] == 0x1)
            {
                if ((Lora_Downward_message[2] == 0xDD) && (Lora_Downward_message[3] == 0xDD))
                {                                                                       //means that device need upload sensor message

                    AMC_Read_Configuration(AMC_Device_Adress, AMC_Register_First_Adress, AMC_Register_Number, AMC_Back_data);
                    flag = AMC_data_proc(AMC_Back_data, (5 + AMC_Register_Number * 2), AMC_Upload_message);
                    while (flag)
                    {                                                                   //verify that the message is correct
                        AMC_Read_Configuration(AMC_Device_Adress, AMC_Register_First_Adress, AMC_Register_Number, AMC_Back_data);
                        flag = AMC_data_proc(AMC_Back_data, (5 + AMC_Register_Number * 2), AMC_Upload_message);
                        puts("get AMC data error\n");                                   //repeatedly get sensor data if the message is wrong
                    }
                    Lora_Data_Send(AMC_Upload_message, Upload_message_len, Gateway_Lora_adress_high, Gateway_Lora_adress_low);
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
                Lora_Data_Send(AMC_Upload_message, Upload_message_len, Gateway_Lora_adress_high, Gateway_Lora_adress_low);
            }
            Uart2_interrupt_flag[0] = 0;                                                
            MemoryWrite32(U2_CTL0_REG, 0x11);                                          //reopen UART2 interrupt enable bit
        }
        
    }
        
    return 0;
}