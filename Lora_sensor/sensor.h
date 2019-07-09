#ifndef __SENSOR_H__
#define __SENSOR_H__

#include"U1_string.h"

char sensor_buff[60];

int get_sensor(char* pc_str,int i_len);
int get_sensor_data(int i_len);
int clean_check_date(char* pc_str);
int Hex_to_Char(int i_hex,char* pc_str);
int proc_data(char* pc_str,int i_len,char* c_re);

#endif