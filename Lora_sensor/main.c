#include<str.h>
#include"U1_string.h"
#include"U2_string.h"
#include"sensor.h"
#include"Lora.h"

#define sensor_len 50

void user_interrupt(){}

int main(){
    U1_strInit();
    U2_strInit();
    char buff[90] = "";
    char c_count;
    int flag;
    while(1){
        getch();
        U2_strSend("1234",4);
        flag = get_sensor(buff,sensor_len);

        if(!flag){
           for(int i = 0;i < sensor_len;++i){
                buff[i] = 0xFF;
            }
        }


        Lora_send(buff,sensor_len);

    }
    return 0;
}