/*--------------------------------------------------------------------
 * TITLE: sensor control function
 * AUTHOR: haikang lihaoqian
 * DATE CREATED: 2019/7/16
 * FILENAME: sensor.h
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/

#ifndef __SENSOR_H__
#define __SENSOR_H__

char sensor_buff[60];

int get_sensor(char* pc_str,int i_len);
int get_sensor_data(char* Sensor_Back_data, int Data_len);
int clean_check_date(char* Sensor_Back_data);
int Hex_to_Char(int i_hex,char* pc_str);
int proc_data(char* Sensor_Back_data,int Data_len,char* Environment_data);

#endif