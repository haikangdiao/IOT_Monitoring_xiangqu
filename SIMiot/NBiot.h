#ifndef __NBIOT_H__
#define __NBIOT_H__

#define nb_timeout 0xF

// char nb_re[3];
// char nb_beg[3];  //test nbiot
// char nb_beg_re[3];
// char nb_cnact[21]; //set nbiot net APN
// char nb_cnact_re[21];  
// char nb_cmauth[71]; //set productkey
// char nb_cmauth_re[3];
// char nb_cmpara[29]; //set activate time
// char nb_cmpara_re[3];
// char nb_cmconn[10];//connect aliyun, need to wait 1m;
// char nb_cmconn_re[3];
// char nb_cmsub[20]; //set push data
// char nb_cmsub_re[3];
// char nb_cmyield[16];  //push state
// char nb_cmtield_re[3];
// char nb_cmpub[60]; // + "data" ,push data. e.g. AT+CMPUB="update",1,"hello"

int nbiot_init();
int nbiot_re_check(char* c_rec,int i_rlen);
// int nbiot_re_check(char* c_rec,int i_rlen,char* c_ch,int i_chlen);
int push_data(char* pc_mes,int i_mlen);

#endif 