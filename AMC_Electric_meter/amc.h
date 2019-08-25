/*--------------------------------------------------------------------
 * TITLE: amc function
 * AUTHOR: haikang
 * DATE CREATED: 2019/8/20
 * FILENAME: amc.h
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/
#ifndef __AMC_H__
#define __AMC_H__

int AMC_CRC(char *Data, int Data_len);
int AMC_Receive(char *AMC_Receive_Data, int Data_len);
int AMC_Read_Configuration(int AMC_Adress, int First_Adress, int Register_Number, char *AMC_Receive_Data);
int AMC_Write_configuration(int AMC_Adress, int First_Adress, int Register_Number, char *Register_Data);
int AMC_data_proc(char *AMC_Back_data, int Data_len, char *AMC_Upload_message);

#endif