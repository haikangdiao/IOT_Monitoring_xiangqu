/*--------------------------------------------------------------------
 * TITLE: Lora command function
 * AUTHOR: haikang
 * DATE CREATED: 2019/7/16
 * FILENAME: Lora.h
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/


#ifndef __LORA_H__
#define __LORA_H__

#include"uart2.h"

/***** lora configuration struct******/
struct Lora_Configuration_{
    char Local_Configuration_Return[80];
    char Remote_Configuration_Return[80];
    char Modify_Configuration_Return[10];
    char Module_Search_Return[80];
    char Module_Reset_Return[10];
    char Destination_Address_add_Return[10];
    char Bridging_Module_Whitelist_Return[10];
} Lora_Configuration_R;

int Read_Local_Configuration(char* Local_Configuration_Return);
int Read_Remote_Configuration(char* Remote_Configuration_Return, int adress_high, int adress_low);
int Modify_Configuration(char* Modify_Configuration_Return, int adress_high, int adress_low, char* NwConfiguration_data);
int Module_Search(char* Module_Search_Return);
int Module_Reset(char* Module_Reset_Return, int adress_high, int adress_low);
int Destination_Address_add(char* Destination_Address_add_Return, int adress_high, int adress_low, int Enable_setting);
int Bridging_Module_Whitelist(char* Bridging_Module_Whitelist_Return, int adress_high, int adress_low, int Whitelist_adress_high, int Whitelist_adress_low);

int Lora_Data_Send(char *pc_str, int data_len, int Destination_adress_high, int Destination_adress_low);
int Lora_test();
int Lora_Data_Receive(char* Receive_data, int data_len);

#endif