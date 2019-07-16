#include<str.h>
#include"uart2.h"
#include"sensor.h"
#include"Lora.h"
#include"func.h"
#include"uart1.h"

#define sensor_len 50

void user_interrupt(){}

int main(){
    U1_Init(); 
    U2_Init();
    char Sensor_Back_data[50];
    char Environment_data[15];
    char c_count;
    int i,flag;

    for (i = 0; i < 100000;i++){}
    puts("test1 begin!\n");
    flag = get_sensor_data(Sensor_Back_data,sensor_len);
    for(i = 0;i < sensor_len;++i){
        puts(xtoa(Sensor_Back_data[i]));
        puts("\n");
    }
    proc_data(Sensor_Back_data, 50, Environment_data);
    // puts("test1 begin!\n");
    // Read_Local_Configuration(Lora_Configuration_R.Local_Configuration_Return);
    // puts("puts the data\n");
    // for (int p_i=0; p_i < 76;p_i++){
    //     puts(xtoa(Lora_Configuration_R.Local_Configuration_Return[p_i]));
        
    // }
    // puts("\n");



    // Modify_Configuration(Lora_Configuration_R.Modify_Configuration_Return, 0x20, 0x02,Lora_Configuration_R.Local_Configuration_Return);
    // for (int p_i=0; p_i < 7;p_i++){
    //     puts(xtoa(Lora_Configuration_R.Modify_Configuration_Return[p_i]));
        
    // }
    // puts("\n");

    // Destination_Address_add(Lora_Configuration_R.Destination_Address_add_Return,0x20, 0x02, 0x01);
    // for (int p_i=0; p_i < 8;p_i++){
    //     puts(xtoa(Lora_Configuration_R.Destination_Address_add_Return[p_i]));
        
    // }
    // puts("\n");

    // Read_Local_Configuration(Lora_Configuration_R.Local_Configuration_Return);
    // for (int p_i=0; p_i < 76;p_i++){
    //     puts(xtoa(Lora_Configuration_R.Destination_Address_add_Return[p_i]));
        
    // }
    // puts("\n");

    Lora_Data_Send(Environment_data, 12, 0x20, 0x01);
    // Lora_Data_Receive(Environment_data, 10);
    // for (int p_i=0; p_i < 10;p_i++){
    //     puts(xtoa(Environment_data[p_i]));
        
    // }
    puts("\n");
    while(1){
        for (i = 0; i < 300000;i++){}
        get_sensor_data(Sensor_Back_data,sensor_len);
        flag=proc_data(Sensor_Back_data, 50, Environment_data);
        if (flag!=0)
        {
            for (int j = 0; j < 6;j++){
                puts(xtoa(Sensor_Back_data[j]));
            }
        }
        
        Lora_Data_Send(Environment_data, 12, 0x20, 0x01);
    }
        
    return 0;
}