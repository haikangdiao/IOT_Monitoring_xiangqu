#include"BC28_MQTT_alibaba.h"
#include"uart2.h"
#include<mcu.h>
#include"Alibaba_iot.h"
#include"func.h"


#define BC28_AT_REBOOT           "AT+NRB\n"
#define BC28_AT_NET_CONN         "AT+CGATT=1\n"
#define BC28_AT_NET_STATUS       "AT+CGATT?\n"
#define BC28_AT_CEDRXS           "AT+CEDRXS=0,5\n"
#define BC28_AT_CPSMS            "AT+CPSMS=0\n"
#define BC28_AT_MQTCFG           "AT+QMTCFG=\"aliauth\",0,"
#define BC28_AT_QMTOPEN          "AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\n"
#define BC28_AT_QMTOPEN_RET      "+QMTOPEN: 0,0"
#define BC28_AT_QMTCONN          "AT+QMTCONN=0,\"M2\"\n"
#define BC28_AT_QMTCONN_RET      "+QMTCONN: 0,0,0"
#define BC28_AT_QMTSUB           "AT+QMTSUB=0,1,"
#define BC28_AT_QMTUNS           "AT+QMTUNS=0,2,"
#define BC28_AT_QMTPUB           "AT+QMTPUB=0,0,0,0,"
#define BC28_AT_QMTDISC          "AT+QMTDISC=0\n"
#define BC28_AT_QMTCLOSE         "AT+QMTCLOSE=0\n"
#define BC28_AT_SUCCESSED        "OK"
#define BC28_AT_FAIL             "ERROR"
#define BC28_AT_URC_QMTRECV      "+QMTRECV:"
#define BC28_AT_URC_QMTSTAT      "+QMTSTAT:"

/****************************
 * @brief
 * This function send AT cmd by the communication link.
 * @param    pc_at_string     pointer of AT cmd string.
 * @param    len              length of string
 * @return                    the length of the sended string
 ****************************/
int HAL_AT_CONN_send(char *pc_at_string, int len){
    DEBUG_STR(pc_at_string);
    return U2_strSend(pc_at_string,len);
}

/****************************
 * @brief
 * This function send AT cmd by the communication link.
 * @param    pc_at_string     pointer of AT cmd string.
 * @param    len              the usable length of the received string
 * @return                    the length of the received string
 ****************************/
int HAL_AT_CONN_recv(char *pc_at_string, int len){
    int tmp = 0;
    while(!(MemoryRead32(U2_DATA_RDY_REG) & 0X01));
    tmp = U2_strRec(pc_at_string,len);
    DEBUG_STR(pc_at_string);
    return tmp;
}

/****************************
 * @brief
 * This function send AT cmd by the communication link.
 * @param    pc_at_string     pointer of AT cmd string.
 * @param    len              the usable length of the received string
 * @param    timeout_MS       timeout that is milliseconds
 * @return                    the length of the received string
 ****************************/
int HAL_AT_CONN_recv_timeout(char *pc_at_string, int len, int timeout_MS){
    int tmp = 0;

    int time = timeout_MS * 140;
    while((!(MemoryRead32(U2_DATA_RDY_REG) & 0X01)) && --time);
    tmp = U2_strRec(pc_at_string,len);
    DEBUG_STR(pc_at_string);
    return tmp;
}

/****************************
 * @brief
 * This function clear received buffer
 * @param         void
 * @return                if 0 is successed
 ****************************/
char HAL_Clear_Rec_buff(){
    // while(1)
    {
        if((MemoryRead32(U2_DATA_RDY_REG) & 0X01)){
            MemoryRead32(U2_READ_REG);
        }else{
            return 0;
        }
    }
    return 0;
}

/*******************************
 * @brief
 * This function determine the status of the return value
 * @param   pc_string       string that is return value of AT cmd and needs to analyze "OK" or "ERROR"
 * @param   len             length of string
 * @return                  0 = "OK"   1 = "ERROR"   2 = NULL
 *******************************/
char BC28_AT_check_status(char* pc_string,int len){
    for(int i = 0;i < len;++i){
        if('O' == pc_string[i] && ((i + 1) < len)){
            if('K' == pc_string[i + 1]){
                return 0;
            }
        }
        if('E' == pc_string[i] && ((i + 4) < len)){
            if('R' == pc_string[i + 1]){
                if('R' == pc_string[i + 2]){
                    if('O' == pc_string[i + 3]){
                        if('R' == pc_string[i + 4]){
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 2;
}

/*****************************
 * @brief
 * This function initial the BC28 module.
 * @param    void
 * @return   if 0 is successed
 *****************************/
char BC28_init(){
    HAL_AT_CONN_init();
    char tmp[10];
    int len = 0;
    char_memset(tmp,0,10);
    HAL_AT_CONN_send("AT\r\n",4);
    HAL_AT_CONN_send("AT\r\n",4);

    // HAL_Clear_Rec_buff();
    HAL_AT_CONN_send("AT\r\n",4);
    len = HAL_AT_CONN_recv(tmp,10);
    return BC28_AT_check_status(tmp,len);
}

/****************************
 * @brief
 * This function reboot BC28 module
 * @param        void
 * @return       if 0 is successed
 ****************************/
char BC28_reboot(){
    HAL_AT_CONN_send(BC28_AT_REBOOT,7);
    return 0;
}

/*******************************
 * @brief 
 * This function set eDRX of BC28, but just supported the first two param
 * @param  mode        Integer type. Disable or enable the use of eDRX in the UE.
 * @param  AcT_type    Integer type. Indicates the type of access technology.
 * @return             if 0 setted successed
 * 
 * AT+CEDRXS=0,5    //close eDRX
 * You can read Quectel_BC35-G&BC28&BC95 R2.0_AT_Commands_Manual_V1.4.pdf to learn more.
 *******************************/
char BC28_setting_eDRX(char mode, char AcT_type, char Requested_eDRX_value){
    char tmp[20];
    int len = 0;
    char_memset(tmp,0,20);
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(BC28_AT_CEDRXS,14);
    len = HAL_AT_CONN_recv(tmp,20);
    return BC28_AT_check_status(tmp,len);
}

/*******************************
 * @brief
 * This function set PSM(Power Saving Mode) of BC28, but only supported the first parm
 * @param   mode        Integer type. Disable or enable the use of PSM in the UE.
 * @return              if 0 setted successed.
 * 
 * AT+CPSMS=0      //close PSM
 */
char BC28_setting_PSM(char mode, char Requested_Periodic_TAU, char Requested_Active_Time){
    char tmp[20];
    int len = 0;
    char_memset(tmp,0,20);
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(BC28_AT_CPSMS,11);
    len = HAL_AT_CONN_recv(tmp,20);
    return BC28_AT_check_status(tmp,len);
}

/******************************
 * @brief
 * This function trigger network attachment
 * @param  status     0 Detached, 1 Attached. Integer type. Indicates the status of PDP context activation.
 * 
 * AT+CGATT=1         
 ******************************/
char BC28_setting_net(char status){
    char tmp[20];
    int len = 0;
    char_memset(tmp,0,20);
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(BC28_AT_NET_CONN,11);
    len = HAL_AT_CONN_recv(tmp,20);
    return BC28_AT_check_status(tmp,len);
}

/*******************************
 * @brief
 * This function check net sate
 * @param    void
 * @return       0 Detached, 1 Attached.
 * 
 * AT+CGATT?     
 */
char BC28_status_net(){
    char tmp[20];
    int len = 0;
    char_memset(tmp,0,20);
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(BC28_AT_NET_STATUS,10);
    len = HAL_AT_CONN_recv(tmp,20);
    return BC28_AT_check_status(tmp,len);
}

/******************************
 * brief
 * This function open MQTT client of BC28 and connect alibaba'IoT
 * @param
 * @return       if 0 is successed.
 * 
 * AT+QMTCFG="aliauth",0,"a16JAzUMgmh","Light001","fUJxnoPDFJazr0eME5wKzCvKyNJwLqq7"
 * AT+QMTOPEN=0,"iot-as-mqtt.cn-shanghai.aliyuncs.com",1883  //wait to return +QMTOPEN: 0,0 
 * AT+QMTCONN=0,"M2"
 ******************************/
char BC28_Alibaba_MQTT_conn(char* product_key, char* device_name, char* device_secret){
    int flag = 0;
    int len = 0;
    char cmd[200];
    char *tmp = cmd;
    const char ret_QMTOPEN[14] = BC28_AT_QMTOPEN_RET;
    const char ret_QMTCONN[16] = BC28_AT_QMTCONN_RET;
    // char_memset(ret_QMTOPEN,0,14);
    // strcpy_u(ret_QMTOPEN,BC28_AT_QMTOPEN_RET);
    // char_memset(ret_QMTCONN,0,16);
    // strcpy_u(ret_QMTCONN,BC28_AT_QMTCONN_RET);

    // DEBUG_STR(product_key);
    // DEBUG_STR(device_name);
    // DEBUG_STR(device_secret);
    // DEBUG_STR(BC28_AT_MQTCFG);

    char_memset(cmd,0,200);
    tmp = strcpy_u(tmp,BC28_AT_MQTCFG);
    tmp = strcpy_u(tmp,"\"");
    tmp = strcpy_u(tmp,PRODUCT_KEY);
    tmp = strcpy_u(tmp,"\",\"");
    tmp = strcpy_u(tmp,DEVICE_NAME);
    tmp = strcpy_u(tmp,"\",\"");
    tmp = strcpy_u(tmp,DEVICE_SECRET);
    tmp = strcpy_u(tmp,"\"\n");

    // DEBUG_STR(cmd);

    //AT+QMTCFG="aliauth",0,"a16JAzUMgmh","Light001","fUJxnoPDFJazr0eME5wKzCvKyNJwLqq7"
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(cmd,strlen_u(cmd));
    len = HAL_AT_CONN_recv(cmd,100);
    // DEBUG_STR(cmd);
    // debug_func(len);
    flag = BC28_AT_check_status(cmd,len);
    if(flag){
        return flag; 
    }

    char_memset(cmd,0,200);
    //AT+QMTOPEN=0,"iot-as-mqtt.cn-shanghai.aliyuncs.com",1883
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(BC28_AT_QMTOPEN,57);
    len = HAL_AT_CONN_recv(cmd,100);
    flag = BC28_AT_check_status(cmd,len);
    if(flag){
        return flag; 
    }
    char_memset(cmd,0,len);
    tmp = HAL_AT_CONN_recv_timeout(cmd,200,600000);
    for(int i = 0;i < tmp;++i){
        if('+' == cmd[i]){
            ++i;
            for(int j = 1;j < 14;++j,++i){
                if(ret_QMTOPEN[j] != cmd[i]){
                    return 1;
                }
            }
        }
    }
    

    char_memset(cmd,0,100);
    //AT+QMTCONN=0,"M2"
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(BC28_AT_QMTCONN,18);
    len = HAL_AT_CONN_recv(cmd,100);
    DEBUG_STR(cmd);
    flag = BC28_AT_check_status(cmd,len);
    if(flag){
        return flag; 
    }
    char_memset(cmd,0,len);
    flag = HAL_AT_CONN_recv_timeout(cmd,200,200000);
    for(int i = 0;i < tmp;++i){
        if('+' == cmd[i]){
            ++i;
            for(int j = 1;j < 14;++j,++i){
                if(ret_QMTCONN[j] != cmd[i]){
                    return 1;
                }
            }
        }
    }

    return 0;
}

/******************************
 * brief
 * This function close MQTT client of BC28
 * @param    void
 * @return           if 0 is successed
 * 
 * AT+QMTDISC=0
 * AT+QMTCLOSE=0
 */
char BC28_Alibaba_MQTT_disconn(){
    char ret[25];
    int len = 0;
    int flag = 0;

    char_memset(ret,0,25);
    //AT+QMTDISC=0
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(BC28_AT_QMTDISC,13);
    len = HAL_AT_CONN_recv(ret,25);
    flag = BC28_AT_check_status(ret,len);
    if(flag){
        return flag;
    }

    char_memset(ret,0,25);
    //AT+QMTCLOSE=0
    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(BC28_AT_QMTCLOSE,14);
    len = HAL_AT_CONN_recv(ret,25);
    return BC28_AT_check_status(ret,len);
}

/******************************
 * brief
 * This function subscribe topic
 * @param topic    need to subscribe topic 
 * @param qos      Quality of Service
 * renturn         if 0 is successed
 * 
 * AT+QMTSUB=0,1,"/a16JAzUMgmh/Light001/user/get",0
 *****************************/
char BC28_Alibaba_MQTT_sub(char *topic, char* qos){
    char ret[60];
    char cmd[60];
    int len;
    char_memset(ret,0,60);
    char_memset(cmd,0,60);

    char* tmp = cmd;
    tmp = strcpy_u(tmp,BC28_AT_QMTSUB);
    tmp = strcpy_u(tmp,"\"");
    tmp = strcpy_u(tmp,topic);
    tmp = strcpy_u(tmp,"\",");
    tmp = strcpy_u(tmp,qos);
    tmp = strcpy_u(tmp,"\n");

    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(cmd,strlen_u(cmd));
    len = HAL_AT_CONN_recv(ret,60);
    return BC28_AT_check_status(ret,len);
}

/******************************
 * brief
 * This function unsubscribe topic
 * @param topic    need to subscribe topic 
 * renturn         if 0 is successed
 * 
 * AT+QMTUNS=0,2,"/a16JAzUMgmh/Light001/user/get"
 *****************************/
char BC28_Alibaba_MQTT_unsub(char *topic){
    char ret[60];
    char cmd[60];
    int len;
    char_memset(ret,0,60);
    char_memset(cmd,0,60);

    char* tmp = cmd;
    tmp = strcpy_u(tmp,BC28_AT_QMTUNS);
    tmp = strcpy_u(tmp,"\"");
    tmp = strcpy_u(tmp,topic);
    tmp = strcpy_u(tmp,"\"\n");

    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(cmd,strlen_u(cmd));
    len = HAL_AT_CONN_recv(ret,60);
    return BC28_AT_check_status(ret,len);
}

/**********************************
 * brief
 * This function pub a message
 * @param topic       topic published 
 * @param msg         message is sent to Alibaba'Iot
 * @param msg_len     length of message
 * @return            if 0 is successed
 * 
 * AT+QMTPUB=0,0,0,0,"/a16JAzUMgmh/Light001/user/update"
 **********************************/
char BC28_Alibaba_MQTT_pub(char *topic, char *msg, int msg_len){
    char msg_ret[200];
    char_memset(msg_ret,0,200);

    char cmd[80];
    int len;
    char_memset(cmd,0,80);

    char* tmp = cmd;
    tmp = strcpy_u(tmp,BC28_AT_QMTPUB);
    tmp = strcpy_u(tmp,"\"");
    tmp = strcpy_u(tmp,topic);
    tmp = strcpy_u(tmp,"\"\n");

    HAL_Clear_Rec_buff();
    HAL_AT_CONN_send(cmd,strlen_u(cmd));
    len = HAL_AT_CONN_recv(cmd,80);
    int i;
    for(i = 0;i < len;++i){
        if('>' == cmd[i]){
            break;
        }
    }
    if(i == len){
        return 1;
    }
    HAL_AT_CONN_send(msg,msg_len);
    HAL_AT_CONN_send(0x1A,1);
    len = HAL_AT_CONN_recv(msg_ret,200);
    return BC28_AT_check_status(msg_ret,len);
}

/************************************
 * @brief
 * This function handle received data
 * @param    msg       return the received data
 * @param    msg_len   the available length of msg
 * @return             if 0 is error!!!!!!!!   
 *                     else length of received data
 *************************************/
int BC28_Alibaba_MQTT_URC(char* msg,int msg_len){
    char rec_len = 0;
    char rec_buff[200];
    const char* pc_URC_MQTRECV = BC28_AT_URC_QMTRECV;
    int len_URC_MQTRECV = strlen_u(BC28_AT_URC_QMTRECV);
    char_memset(rec_buff,0,rec_len);
    char_memset(msg,0,msg_len);

    int i,j,h;
    rec_len = HAL_AT_CONN_recv(rec_buff,200);
    for(i = 0;i < rec_len;++i){
        if('+' == rec_buff[i] && (i + 8 < rec_len)){
            ++i;
            for(j = 1;j < len_URC_MQTRECV;++j,++i){
                if(pc_URC_MQTRECV[j] != rec_buff[i]){
                    return 0;
                }
            }
            if(j == len_URC_MQTRECV){
                for(h = 0;(h < rec_buff - i) && (h < msg_len);++h){
                    msg[h] = rec_buff[i + h];
                }
                return h;
            }          
        }
    }
    return 0;
}



int BC28_Aliyun_connect(){

    debug_func(BC28_init());
    debug_func(BC28_setting_eDRX(0,5,0));
    debug_func(BC28_setting_PSM(0,0,0));
    debug_func(BC28_setting_net(0));
    debug_func(BC28_status_net());
    debug_func(BC28_Alibaba_MQTT_conn(PRODUCT_KEY,DEVICE_NAME,DEVICE_SECRET));
    debug_func(BC28_Alibaba_MQTT_sub(TOPIC_GET,"0"));
    debug_func(BC28_Alibaba_MQTT_pub(TOPIC_UPDATE,"I`m rebot",9));

    return 0;
}