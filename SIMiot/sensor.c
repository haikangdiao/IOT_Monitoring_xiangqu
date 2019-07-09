#include"sensor.h"
#include"U1_string.h"
#include<str.h>

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
    // while(tens){
    //     tmp = i_hex / tens;
    //     if(tmp){
    //         pc_str[pos] = tmp + 0x30;
    //         i_hex -= tmp * tens;
    //         ++pos;
    //     }
    //     tens /= 10;
    // }
    pc_str[pos] = '\0';
    return pos;
}

int proc_data(char* pc_str,int i_len,char* c_re){
    int pos = 0; 
    int tmp;
    // for(int i = 0;i < 43;++i){
    //         putch(pc_str[i]);
    //     }

    //处理温度
    if(0x22 == pc_str[6]){
        if(0x00 == pc_str[7]){
            if(0x01 == pc_str[8]){
                if(0x02 == pc_str[9]){
                    // putch(pc_str[10]);
                    // putch(pc_str[11]);
                    tmp = (0x000000FF & pc_str[11]) | (0x0000FF00 & (pc_str[10] << 8));
                    if(0x7F < pc_str[10]){
                        tmp = 0xFFFF0000 | tmp;
                    }
                    // putch(tmp);
                    // putch(tmp >> 8);
                    // putch(tmp >> 16);
                    // putch(tmp >> 24);

                    // char buff[5] = "\0";
                    // int z = 0;
                    pos += Hex_to_Char(tmp,c_re + pos);
                    c_re[pos + 1] = ',';
                    c_re[pos] = c_re[pos - 1];
                    c_re[pos - 1] = '.';
                    pos += 2;
                    // for(int i = 0;i < pos;++i){
                    //     putch(c_re[i]);
                    // }

                    // int i_count = Hex_to_Char(0x00EB,buff);
                    // for(int i =0;i < i_count;++i){
                    //     putch(buff[i]);
                    // }
                }
            }
        }
    }

    //处理湿度
    if(0x22 == pc_str[12]){
        if(0x00 == pc_str[13]){
            if(0x03 == pc_str[14]){
                if(0x02 == pc_str[15]){
                    tmp = (0x000000FF & pc_str[17]) | (0x0000FF00 & (pc_str[16] << 8));
                    if(0x7F < pc_str[16]){
                        tmp = 0xFFFF0000 | tmp;
                    }
                    // putch(tmp);
                    // putch(tmp >> 8);
                    // putch(tmp >> 16);
                    // putch(tmp >> 24);

                    pos += Hex_to_Char(tmp,c_re + pos);
                    c_re[pos + 2] = ',';
                    c_re[pos + 1] = '%';
                    c_re[pos] = c_re[pos - 1];
                    c_re[pos - 1] = '.';
                    pos += 3;
                }
            }
        }
    }

    //处理CO2
    if(0x22 == pc_str[24]){
        if(0x00 == pc_str[25]){
            if(0x06 == pc_str[26]){
                if(0x02 == pc_str[27]){
                    tmp = (0x000000FF & pc_str[29]) | (0x0000FF00 & (pc_str[28] << 8));
                    if(0x7F < pc_str[28]){
                        tmp = 0xFFFF0000 | tmp;
                    }
                    // putch(tmp);
                    // putch(tmp >> 8);
                    // putch(tmp >> 16);
                    // putch(tmp >> 24);
                    
                    pos += Hex_to_Char(tmp,c_re + pos);
                    c_re[pos] = ',';
                    ++pos;
                }
            }
        }
    }
    return pos;
}

int clean_check_date(char* pc_str){
    int i,j,z;
    char checksum = 0;
    for(i = 0;i < 5;++i){
        if(0xEF == sensor_buff[i]){
            if(0xFE == sensor_buff[i + 1]){
                for(j = 0;j < 49;++j,++i){
                    pc_str[j] = sensor_buff[i];
                    checksum += pc_str[j];
                }
                pc_str[j] = sensor_buff[i];
                break;
            }
        }
    }
    for(z = 0;z < 50;++z){
        putch(pc_str[z]);
    }
    putch(checksum);
    if(checksum == pc_str[z - 1]){
        return 1;
    }else{
        return 0;
    }
}


void get_sensor_data(int i_len){
    char c_sensor_cmd[6] = {0xEF,0xFE,0x01,0x02,0xF0};
    puts(c_sensor_cmd);
    U1_strSend(c_sensor_cmd,5);
    U1_strRec(sensor_buff,i_len);
    for(int i = 0;i < 55;++i){
        putch(sensor_buff[i]);
    }
}

int get_sensor(char* pc_str,int i_len){
    int i_count = 10;
    do{
        get_sensor_data(i_len + 5);
    }while(!clean_check_date(pc_str) && --i_count);
    if(i_count){
        return 1;
    }else{
        return 0;
    }
}