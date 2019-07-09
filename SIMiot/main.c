#include "uart1.h"
#include "U1_string.h"
#include "sensor.h"
#include "uart2.h"
#include "U2_string.h"
#include "NBiot.h"
#include <mcu.h>
#include <str.h>

#define sensor_len 50

// extern char nb_re[3];
// extern char nb_beg[3];  //test nbiot
// extern char nb_beg_re[3];
// extern char nb_cnact[21]; //set nbiot net APN
// extern char nb_cnact_re[21];  
// extern char nb_cmauth[71]; //set productkey
// extern char nb_cmauth_re[3];
// extern char nb_cmpara[29]; //set activate time
// extern char nb_cmpara_re[3];
// extern char nb_cmconn[10];//connect aliyun, need to wait 1m;
// extern char nb_cmconn_re[3];
// extern char nb_cmsub[20]; //set push data
// extern char nb_cmsub_re[3];
// extern char nb_cmyield[16];  //push state
// extern char nb_cmtield_re[3];
// extern char nb_cmpub[60]; // + "data" ,push data. e.g. AT+CMPUB="update",1,"hello"


void user_interrupt(){}

int main(){
    U1_Init();
    U2_Init();
    char c_ch;
    char buff[100] = {0};
    // char c_sensor_cmd[8] = {0xEF,0xFE,0x01,0x01,0xF0,0x0D,0x0A};
    int i_count = 0;
    int flag = 0;
    // char c_re[50] = {0xEF,0xFE,0x01,0x06,0x31,0x00,0x22,0x00,0x01,0x02,0x00,0x82,0x22,0x00,0x03,0x02,0x02,0x3A,0x22,0x00,0x05,0x02,0x00,0x18,0x22,0x00,0x06,0x02,0x0C,0xD9,0x22,0x00,0x09,0x02,0x00,0x00,0x22,0x00,0x04,0x02,0x00,0x00,0x22,0x00,0x07,0x02,0x00,0x4E,0x00,0x4D};
    char sensor_data[20] = {0};
    nbiot_init();
    while(1)
    {
        // c_ch = getch();

        // for(int i = 0;i < 43;++i){
        //     putch(c_re[i]);
        // }

        flag = get_sensor(buff,sensor_len);
        // putch(flag);
        // for(int i =0;i < sensor_len;++i){
        //     putch(buff[i]);
        // }

        i_count = proc_data(buff,sensor_len,sensor_data);
        putch(i_count);
        for(int i = 0;i < i_count;++i){
            putch(sensor_data[i]);
        }

        push_data(sensor_data,i_count);

        for(int i = 0;i < 8;++i){
            delay();
        }

        // i_count = Hex_to_Char(0x00EB,buff);
        // for(int i =0;i < i_count;++i){
        //     putch(buff[i]);
        // }
        // putch('\n');
        // i_count = Hex_to_Char(0x00,buff);
        // for(int i =0;i < i_count;++i){
        //     putch(buff[i]);
        // }
        // putch('\n');        
        // i_count = Hex_to_Char(0xFFFF0000 | 0xFF15,buff);
        // for(int i =0;i < i_count;++i){
        //     putch(buff[i]);
        // }
        // putch('\n');

        // nbiot_init();

        // flag = get_sensor(buff,sensor_len);
        // putch(flag);
        // for(int i =0;i < sensor_len;++i){
        //     putch(buff[i]);
        // }

        

        // getch();
        // push_data(buff,sensor_len);

       

        // U2_strSend(c_sensor_cmd,7);
        // i_count = U2_strRec(buff,80);
        // U2_strSend(buff,i_count);
        // for(int i =0;i < sensor_len;++i){
        //     putch(buff[i]);
        // }

        // U2_strSend(c_sensor_cmd,7);

        //U2_Send(&c_ch);

        // while(MemoryRead32(U2_BUSY_REG)){}
        // MemoryWrite32(U2_WRITE_REG,c_ch);

        //U2_Send(c_sensor_cmd);

        // for(int i = 0;i < 79;++i){
        //     buff[i] = U2_Rec();
        // }
        // for(int j = 0;j < 79;++j){
        //     U2_Send(buff + j);
        // }

        // flag = get_sensor(buff,sensor_len);
        // putch(flag);

        // get_sensor_data(sensor_len);
        // flag = clean_check_date(buff);
        // putch(flag);
        //U1_strSend(buff,79);

        // U1_strSend(c_sensor_cmd,7);
        // i_count = U1_strRec(buff,80);
        // U1_strSend(buff,i_count);
        // for(int i =0;i < sensor_len;++i){
        //     putch(buff[i]);
        // }

        
        // for(int i = 0;i < 79;++i){
        //     while(!(MemoryRead32(U1_DATA_RDY_REG) & 0X01)){}
        //     buff[i] = MemoryRead32(U1_READ_REG);
        // }
        // for(int j = 0;j < 79;++j){
        //     c_ch = buff[j];
        //     while(MemoryRead32(U1_BUSY_REG)){}
        //     MemoryWrite32(U1_WRITE_REG,c_ch);
        // }
        // while(!(MemoryRead32(U1_DATA_RDY_REG) & 0X01)){}
        // c_ch = MemoryRead32(U1_READ_REG);
        // while(MemoryRead32(U1_BUSY_REG)){}
        // MemoryWrite32(U1_WRITE_REG,MemoryRead32(U1_DATA_RDY_REG));
        // while(MemoryRead32(U1_BUSY_REG)){}
        // MemoryWrite32(U1_WRITE_REG,c_ch);

        //char buff[100] = {0};
        //for(int i = 0;i < 80;++i){
        //    buff[i] = U1_Rec();
        //}
        //for(int j = 0;j < 80;++j){
        //    U1_Send(buff + j);
        //}
        //char c_ch;
        //c_ch = U1_Rec();
        //putch(c_ch);
        //char c_ch;
        //c_ch = getch();
        //c_ch = U1_Rec();
        //putch(c_ch);
        //U1_Send(&c_ch);
        //char c_sensor_cmd[8] = {0xEF,0xFE,0x01,0x01,0xF0,0x0D,0x0A};
        //U1_strSend(c_sensor_cmd,7);

        //int i = 0;
        //char buff[100] = {};
        //i = U1_strRec(buff,100);
        //puts(buff);
        //U1_strSend(buff,i);

        //getch();
        //char buff[100] = {0};
        //get_sensor(buff,100);
        //U1_strSend(buff,50);
        //puts(xtoa(buff));

        //char c_sensor_cmd[8] = {0xEF,0xFE,0x01,0x01,0xF0,0x0D,0x0A};
        //for(int i = 0;i < 7;++i){
        //    U1_Send(c_sensor_cmd + i);
        //}
        //for(int j = 0;j < 100;++j){
         //   putch(U1_Rec());
        //}
        //puts(c_sensor_cmd);

    }
    return 0;
}