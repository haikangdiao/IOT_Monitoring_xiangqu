/*--------------------------------------------------------------------
 * TITLE: sensor control function
 * AUTHOR: haikang lihaoqian
 * DATE CREATED: 2019/7/16
 * FILENAME: sensor.c
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/

#include"sensor.h"
#include"str.h"
#include "uart1.h"


int Hex_to_Char(int i_hex,char* pc_str){
    int pos = 0;
    if(0 == i_hex){
        pc_str[pos] = '0';
        pc_str[pos + 1] = '\0';
        return 1;
    }
    if(i_hex < 0){
        i_hex = -i_hex;
        pc_str[pos] = '-';
        ++pos;
    }
    int tens = 1000000000;
    int tmp = 0;
    while(!(i_hex / tens)){
        tens /= 10;
    }
     while(tens){
        tmp = i_hex / tens;
        pc_str[pos] = tmp + 0x30;
        i_hex -= tmp * tens;
        ++pos;
        tens /= 10;
    }
    pc_str[pos] = '\0';
    return pos;
}

/**
 * @brief 
 * This function is to process the data from the sensor
 * @param Sensor_Back_data              the data from the sensor 
 * @param Data_len                      length of the data  
 * @param Sensor_Upload_message         the data that was processed        
 * @return                              the flag that Whether the function is successfully executed. 0 is successfully
 */
int proc_data(char* Sensor_Back_data,int Data_len,char* Sensor_Upload_message){
    int Check_Byte = 0; 
    int Temp,Humi,PM2_5,CO2,VOC,HCHO;

    if ((Sensor_Back_data[6]==0x22) && (Sensor_Back_data[7]==0x00) && (Sensor_Back_data[8]==0x01) && (Sensor_Back_data[9]==0x02))
    {
        Temp=(0x000000FF & Sensor_Back_data[11]) | (0x0000FF00 & (Sensor_Back_data[10] << 8));
        Sensor_Upload_message[7] = Sensor_Back_data[8];
        Sensor_Upload_message[8] = Sensor_Back_data[10];
        Sensor_Upload_message[9] = Sensor_Back_data[11];
    }
    else{
        puts("Temperature data is wrong\n");
        return 1;
    }
        

    if ((Sensor_Back_data[12]==0x22) && (Sensor_Back_data[13]==0x00) && (Sensor_Back_data[14]==0x03) && (Sensor_Back_data[15]==0x02))
    {
        Humi=(0x000000FF & Sensor_Back_data[17]) | (0x0000FF00 & (Sensor_Back_data[16] << 8));
        Sensor_Upload_message[10] = Sensor_Back_data[14];
        Sensor_Upload_message[11] = Sensor_Back_data[16];
        Sensor_Upload_message[12] = Sensor_Back_data[17];
    }
    else{
        puts("Humidity data is wrong\n");
        return 2;
    }
        

    if ((Sensor_Back_data[18]==0x22) && (Sensor_Back_data[19]==0x00) && (Sensor_Back_data[20]==0x05) && (Sensor_Back_data[21]==0x02))
    {
        PM2_5=(0x000000FF & Sensor_Back_data[23]) | (0x0000FF00 & (Sensor_Back_data[22] << 8));
        Sensor_Upload_message[13] = Sensor_Back_data[20];
        Sensor_Upload_message[14] = Sensor_Back_data[22];
        Sensor_Upload_message[15] = Sensor_Back_data[23];
    }
    else{
        puts("PM2_5 data is wrong\n");
        return 3;
    }
        

    if ((Sensor_Back_data[24]==0x22) && (Sensor_Back_data[25]==0x00) && (Sensor_Back_data[26]==0x06) && (Sensor_Back_data[27]==0x02))
    {
        CO2=(0x000000FF & Sensor_Back_data[29]) | (0x0000FF00 & (Sensor_Back_data[28] << 8));
        Sensor_Upload_message[16] = Sensor_Back_data[26];
        Sensor_Upload_message[17] = Sensor_Back_data[28];
        Sensor_Upload_message[18] = Sensor_Back_data[29];
    }
    else{
        puts("CO2 data is wrong\n");
        return 4;
    }
        

    if ((Sensor_Back_data[30]==0x22) && (Sensor_Back_data[31]==0x00) && (Sensor_Back_data[32]==0x09) && (Sensor_Back_data[33]==0x02))
    {
        VOC=(0x000000FF & Sensor_Back_data[35]) | (0x0000FF00 & (Sensor_Back_data[34] << 8));
        Sensor_Upload_message[19] = Sensor_Back_data[32];
        Sensor_Upload_message[20] = Sensor_Back_data[34];
        Sensor_Upload_message[21] = Sensor_Back_data[35];
    }
    else{
        puts("VOC data is wrong\n");
        return 5;
    }
        

    if ((Sensor_Back_data[42]==0x22) && (Sensor_Back_data[43]==0x00) && (Sensor_Back_data[44]==0x07) && (Sensor_Back_data[45]==0x02))
    {
        HCHO=(0x000000FF & Sensor_Back_data[47]) | (0x0000FF00 & (Sensor_Back_data[46] << 8));
        Sensor_Upload_message[22] = Sensor_Back_data[44];
        Sensor_Upload_message[23] = Sensor_Back_data[46];
        Sensor_Upload_message[24] = Sensor_Back_data[47];
    }
    else{
        puts("HCHO data is wrong\n");
        return 6;
    }
    Sensor_Upload_message[4] = 20;
    for (int i = 0; i < Sensor_Upload_message[4];i++){
        Check_Byte = Sensor_Upload_message[5 + i] + Check_Byte;
    }
    
    Sensor_Upload_message[25] = Check_Byte;
    Sensor_Upload_message[26] = 0XEF;
    Sensor_Upload_message[27] = 0XEF;     //This is data part of message that received from the sensor
    puts("\nsensor data processed\n");
    return 0;
}

/**
 * @brief 
 * This function is to get the data from the sensor
 * @param Sensor_Back_data              the data from the sensor 
 * @param Data_len                      length of the data         
 * @return                              the flag that Whether the function is successfully executed. 0 is successful
 */
int get_sensor_data(char* Sensor_Back_data, int Data_len){
    U1_SendInt(0xEF);
	U1_SendInt(0xFE);
	U1_SendInt(0x01);
	U1_SendInt(0x02);
	U1_SendInt(0xF0);
    U1_strRec(Sensor_Back_data, 50);
    if((Sensor_Back_data[0]==0xFF) && (Sensor_Back_data[1]==0xFF) && (Sensor_Back_data[2]==0xEF) && (Sensor_Back_data[3]==0xFE)){
        for (int sensor_i=0; sensor_i < 48;sensor_i++){
            Sensor_Back_data[sensor_i] = Sensor_Back_data[sensor_i + 2];
        }
    }       //make sure that the data starts with 0xEFFE
    
    return 0;
}
