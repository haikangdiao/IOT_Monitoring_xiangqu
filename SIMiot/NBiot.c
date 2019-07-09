#include"NBiot.h"
#include"U2_string.h"
#include"func.h"
#include<str.h>


//检查是否回复了OK
int nbiot_re_check(char* c_rec,int i_rlen){
    int i = 0;
    int len = i_rlen - 1;
    for(i = 0;i < len;++i){
        if('O' == c_rec[i]){
            if('K' == c_rec[i + 1]){
                return 1;
            }
        }
    }
    return 0;
}

// //按照要求检查字符
// int nbiot_re_check(char* c_rec,int i_rlen,char* c_ch,int i_chlen){
//     int i = 0,j = 0;
//     int len = i_rlen - i_chlen;
//     for(i = 0,j = 0;i < len;++i){
//         if(c_ch[j] == c_rec[i]){
//             ++j;
//             if(j == i_chlen - 1){
//                 return 1;
//             }
//         }else{
//             j = 0;
//         }
//     }
//     return 0;
// }

//发送一次指令，并判断是否成功
int nbiot_send_ret(char* c_ch,int i_clen,char* c_ret,int i_rlen){
    int i_count = nb_timeout;
    for(int j = 0;j < i_clen;++j){
        putch(c_ch[j]);
    }
    //发送命令，并等待回复和判断
    U2_strSend(c_ch,i_clen);
    do{
        putch(i_count);
        U2_strRec(c_ret,i_rlen);
    }while(!nbiot_re_check(c_ret,i_rlen) && --i_count);
    
    //如果没有回复OK，就返回0
    if(!i_count){
        return 0;
    }

    for(int i = 0;i < i_rlen;++i){
        putch(c_ret[i]);
    }
    // putch(i_count);

    return 1;
}

int push_data(char* pc_mes,int i_mlen){
    char nb_cmpub[60] = "AT+CMPUB=\"update\",1,\""; // + "data" ,push data. e.g. AT+CMPUB="update",1,"hello"
    char nb_rec[90] = "";

    //拼接数据
    strcomb(nb_cmpub + 21,pc_mes,i_mlen);
    strcomb(nb_cmpub + 21 + i_mlen,"\"\r\r\n",5);

    for(int i = 0;i < 26 + i_mlen;++i){
        putch(nb_cmpub[i]);
    }

    //push data
    int flag = 0;
    int i_count = 4;
    do{
        putch(i_count);
        flag = nbiot_send_ret(nb_cmpub,26 + i_mlen,nb_rec,50);
    }while(!flag && --i_count);
    if(!i_count){
        return 0;
    }

    return 26 + i_mlen;
}

int nbiot_init(){
    //char nb_re[3] = "OK";
    char nb_beg[6] = "AT\r\r\n";  //test nbiot
    //char nb_beg_re[3] = "OK";
    char nb_cnact[24] = "AT+CNACT=1,\"cmnbiot\"\r\r\n"; //set nbiot net APN
    //char nb_cnact_re[21] = "OK\n\n+APP PDP: ACTIVE";  
    //char nb_cmauth[74] = "AT+CMAUTH=\"a1Ex1aw7XM5\",\"SIMiot001\",\"ROxw2w7eeN8JyIhlQJJfhgPGu2vbRcwz\"\r\r\n"; //set productkey
     char nb_cmauth[74] = "AT+CMAUTH=\"a1WO4eQqrLT\",\"NxK1iWAs35jGjHR7EzsL\",\"Wtz1l6mRANAjM1mM8OIRCgBDWwIHXSpg\"\r\r\n"; //set productkey
    //char nb_cmauth_re[3] = "OK";
    char nb_cmpara[32] = "AT+CMPARA=\"KEEPALIVE\",180000\r\r\n"; //set activate time
    //char nb_cmpara_re[3] = "OK";
    char nb_cmconn[13] = "AT+CMCONN\r\r\n";//connect aliyun, need to wait 1m;
    //char nb_cmconn_re[3] = "OK";
    char nb_cmsub[23] = "AT+CMSUB=\"update\",1\r\r\n"; //set push data
    //char nb_cmsub_re[3] = "OK";
    char nb_cmyield[19] = "AT+CMYIELD=2000\r\r\n";  //push state
    //char nb_cmtield_re[3] = "OK";
    //char nb_cmpub[60] = "AT+CMPUB=\"update\",1,\"hello\"\r\r\n"; // + "data" ,push data. e.g. AT+CMPUB="update",1,"hello"
    char nb_rec[85] = "";
    // for(int i = 0;i < 21;++i){
    //     putch(nb_cnact_re[i]);
    // }
    // for(int j = 0;j < 3;++j){
    //     putch(nb_re[j]);
    // }

    int i_count = 0;
    int flag = 0;

    //同步连接
    U2_strSend(nb_beg,6);
    delay();
    U2_strSend(nb_beg,6);
    delay();
    U2_strSend(nb_beg,6);
    delay();
    U2_strSend(nb_beg,6);
    delay();
    U2_strSend(nb_beg,6);
    delay();
    U2_strSend(nb_beg,6);
    delay();

    getch();
    //测试连通情况，
    flag = 0;
    i_count = 4;
    do{
        putch(i_count);
        flag = nbiot_send_ret(nb_beg,6,nb_rec,10);
    }while(!flag && --i_count);
    if(!i_count){
        return 0;
    }

    // getch();
    //设置APN
    flag = 0;
    i_count = 4;
    do{
        putch(i_count);
        flag = nbiot_send_ret(nb_cnact,24,nb_rec,30);
    }while(!flag && --i_count);
    if(!i_count){
        return 0;
    }

    // getch();
    //设置产品Key
    flag = 0;
    i_count = 4;
    do{
        putch(i_count);
        flag = nbiot_send_ret(nb_cmauth,74,nb_rec,80);
    }while(!flag && --i_count);
    if(!i_count){
        return 0;
    }

    // getch();
    //设置激活时间
    flag = 0;
    i_count = 4;
    do{
        putch(i_count);
        flag = nbiot_send_ret(nb_cmpara,32,nb_rec,40);
    }while(!flag && --i_count);
    if(!i_count){
        return 0;
    }

    // getch();
    //连接服务器，等待时间稍长
    flag = 0;
    i_count = 4;
    do{
        putch(i_count);
        flag = nbiot_send_ret(nb_cmconn,13,nb_rec,19);
    }while(!flag && --i_count);
    if(!i_count){
        return 0;
    }

    // getch();
    //订阅服务
    flag = 0;
    i_count = 4;
    do{
        putch(i_count);
        flag = nbiot_send_ret(nb_cmsub,23,nb_rec,30);
    }while(!flag && --i_count);
    if(!i_count){
        return 0;
    }

    // getch();
    //查询订阅是否成功
    flag = 0;
    i_count = 4;
    do{
        putch(i_count);
        flag = nbiot_send_ret(nb_cmyield,19,nb_rec,25);
    }while(!flag && --i_count);
    if(!i_count){
        return 0;
    }

    // getch();
    // push_data("hello",5);

    // getch();
    // //push data
    // flag = 0;
    // i_count = 4;
    // do{
    //     putch(i_count);
    //     flag = nbiot_send_ret(nb_cmpub,31,nb_rec,40);
    // }while(!flag && --i_count);
    // if(!i_count){
    //     return 0;
    // }
    
    // getch();
    // nbiot_send_ret(nb_cnact,22,nb_rec,30);
    // getch();
    // nbiot_send_ret(nb_cmauth,72,nb_rec,80);
    // getch();
    // nbiot_send_ret(nb_cmpara,30,nb_rec,40);
    // getch();
    // nbiot_send_ret(nb_cmconn,11,nb_rec,19);
    // getch();
    // nbiot_send_ret(nb_cmsub,21,nb_rec,30);
    // getch();
    // nbiot_send_ret(nb_cmyield,17,nb_rec,25);
    // getch();
    // nbiot_send_ret(nb_cmpub,31,nb_rec,40);

    // getch();
    // U2_strSend(nb_beg,6);
    // getch();
    // U2_strSend(nb_cnact,22);
    // getch();
    // U2_strSend(nb_cmauth,72);
    // getch();
    // U2_strSend(nb_cmpara,30);
    // getch();
    // U2_strSend(nb_cmconn,11);
    // getch();
    // U2_strSend(nb_cmsub,21);
    // getch();
    // U2_strSend(nb_cmyield,17);
    // getch();
    // U2_strSend(nb_cmpub,31);

    // flag = nbiot_re_check(nb_cnact_re,21);
    // putch(flag);

    return 1;
}