#ifndef __BC28_MQTT_ALIBABA__
#define __BC28_MQTT_ALIBABA__

int BC28_status_flag;

/****************************
 * @brief
 * This function initial the communication link for AT cmd.
 * @param        void
 * @return       void
 ****************************/
void HAL_AT_CONN_init();

/****************************
 * @brief
 * This function send AT cmd by the communication link.
 * @param    pc_at_string     pointer of AT cmd string.
 * @param    len              length of string
 * @return                    the length of the sended string
 ****************************/
int HAL_AT_CONN_send(char *pc_at_string, int len);

/****************************
 * @brief
 * This function send AT cmd by the communication link.
 * @param    pc_at_string     pointer of AT cmd string.
 * @param    len              the usable length of the received string
 * @return                    the length of the received string
 ****************************/
int HAL_AT_CONN_recv(char *pc_at_string, int len);

/****************************
 * @brief
 * This function send AT cmd by the communication link.
 * @param    pc_at_string     pointer of AT cmd string.
 * @param    len              the usable length of the received string
 * @param    timeout_MS       timeout that is milliseconds
 * @return                    the length of the received string
 ****************************/
int HAL_AT_CONN_recv_timeout(char *pc_at_string, int len, int timeout_MS);

/****************************
 * @brief
 * This function clear received buffer
 * @param         void
 * @return                if 0 is successed
 ****************************/
char HAL_Clear_Rec_buff();

/*******************************
 * @brief
 * This function determine the status of the return value
 * @param   pc_string       string that is return value of AT cmd and needs to analyze "OK" or "ERROR"
 * @param   len             length of string
 * @return                  1 = "OK"   0 = "ERROR"   2 = NULL
 *******************************/
char BC28_AT_check_status(char* pc_string,int len);

/*****************************
 * @brief
 * This function initial the BC28 module.
 * @param    void
 * @return   if 0 is successed
 *****************************/
char BC28_init();

/****************************
 * @brief
 * This function reboot BC28 module
 * @param        void
 * @return       if 0 is successed
 ****************************/
char BC28_reboot();

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
char BC28_setting_eDRX(char mode, char AcT_type, char Requested_eDRX_value);

/*******************************
 * @brief
 * This function set PSM(Power Saving Mode) of BC28, but only supported the first parm
 * @param   mode        Integer type. Disable or enable the use of PSM in the UE.
 * @return              if 0 setted successed.
 * 
 * AT+CPSMS=0      //close PSM
 */
char BC28_setting_PSM(char mode, char Requested_Periodic_TAU, char Requested_Active_Time);

/******************************
 * @brief
 * This function trigger network attachment
 * @param  status     0 Detached, 1 Attached. Integer type. Indicates the status of PDP context activation.
 * 
 * AT+CGATT=1         
 ******************************/
char BC28_setting_net(char status);

/*******************************
 * @brief
 * This function check net sate
 * @param    void
 * @return       0 Detached, 1 Attached.
 * 
 * AT+CGATT?     
 */
char BC28_status_net();


/******************************
 * brief
 * This function open MQTT client of BC28 and connect alibaba'IoT
 * @param
 * @return       if 0 is successed.
 * 
 * AT+QMTCFG="aliauth",0,"a16JAzUMgmh","Light001","fUJxnoPDFJazr0eME5wKzCvKyNJwLqq7"
 * AT+QMTOPEN=0,"iot-as-mqtt.cn-shanghai.aliyuncs.com",1883  //wait to return +QMTOPEN: 0,0 
 * AT+QMTCONN=0,"Light001"
 ******************************/
char BC28_Alibaba_MQTT_conn(char* product_key, char* device_name, char* device_secret);

/******************************
 * brief
 * This function close MQTT client of BC28
 * @param    void
 * @return           if 0 is successed
 * 
 * AT+QMTDISC=0
 * AT+QMTCLOSE=0
 */
char BC28_Alibaba_MQTT_disconn();

/******************************
 * brief
 * This function subscribe topic
 * @param topic    need to subscribe topic 
 * @param qos      Quality of Service
 * renturn         if 0 is successed
 * 
 * AT+QMTSUB=0,1,"/a16JAzUMgmh/Light001/user/get",0
 *****************************/
char BC28_Alibaba_MQTT_sub(char *topic, char* qos);

/******************************
 * brief
 * This function unsubscribe topic
 * @param topic    need to subscribe topic 
 * renturn         if 0 is successed
 * 
 * AT+QMTUNS=0,2,"/a16JAzUMgmh/Light001/user/get"
 *****************************/
char BC28_Alibaba_MQTT_unsub(char *topic);

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
char BC28_Alibaba_MQTT_pub(char *topic, char *msg, int msg_len);

/************************************
 * @brief
 * This function handle received data
 * @param    msg       return the received data
 * @param    msg_len   the available length of msg
 * @return             if 0 is error!!!!!!!!   
 *                     else length of received data
 *************************************/
int BC28_Alibaba_MQTT_URC(char* msg,int msg_len);

#endif