#ifndef __SENSOR_H__
#define __SENSOR_H__

char sensor_buff[100];

int get_sensor(char* pc_str,int i_len);
void get_sensor_data(int i_len);
int clean_check_date(char* pc_str);
int Hex_to_Char(int i_hex,char* pc_str);
int proc_data(char* pc_str,int i_len,char* c_re);

#endif