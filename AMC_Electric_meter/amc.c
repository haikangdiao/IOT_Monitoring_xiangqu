/*--------------------------------------------------------------------
 * TITLE: amc function
 * AUTHOR: haikang
 * DATE CREATED: 2019/8/20
 * FILENAME: amc.c
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/
#include "amc.h"
#include "uart1.h"
#include "str.h"



/**
 * @brief 
 * This function is to generate CRC check code
 * @param Data                          the data need to be generated CRC check code
 * @param Data_len                      the length of data             
 * @return                              the CRC check code
 */
int AMC_CRC(char* Data, int Data_len)
{
    unsigned int CRC_Check = 0xFFFF;
    unsigned int data,temp;
    data = 0;
    //generate CRC check code, follow the instruction to achieve
    for (int i = 0; i < Data_len;i++){
        data = Data[i] + 0;
        CRC_Check = data ^ CRC_Check;
        for (int j = 0; j < 8;j++){
            temp = CRC_Check & 0x01;
            //CRC_Check = CRC_Check >> 1;
            CRC_Check = CRC_Check / 2;
            
            if(temp==1)
                CRC_Check = CRC_Check ^ 0xA001;
        }   
    }
    return CRC_Check;
}

/**
 * @brief 
 * This function is to receive the data from electric meter
 * @param AMC_Receive_Data              the data received from electric meter
 * @param Data_len                      the length of data             
 * @return                              the flag data. 0 is successfully,1 means receiving data error,2 means CRC error
 */
int AMC_Receive(char* AMC_Receive_Data, int Data_len){
    unsigned int i,CRC_Check;
    i = U1_strRec(AMC_Receive_Data, Data_len);
    if(i!=Data_len)
    {
        puts(xtoa(i));
        return 1;
    }
        
    CRC_Check = AMC_CRC(AMC_Receive_Data, (Data_len - 2));      //generate CRC check code
    i = AMC_Receive_Data[Data_len - 1];                         //get CRC check code from the data
    i = (i << 8) + AMC_Receive_Data[Data_len - 2];
    if(i!=CRC_Check){
        return 2;
    }                                            //compare CRC check code
        
    return 0;
}

/**
 * @brief 
 * This function is to read register data from electric meter
 * @param AMC_Adress                    the adress of AMC electric meter 
 * @param First_Adress                  the first adress of register that you want to read
 * @param Register_Number               the number of register you want to read
 * @param AMC_Receive_Data              the data received from electric meter
 * @return                              the flag data. 1 is successfully
 */
int AMC_Read_Configuration(int AMC_Adress, int First_Adress, int Register_Number, char* AMC_Receive_Data){
    char Read_data[9];
    int CRC_Check = 0;
    unsigned int i,j;
    i = j = 0;
    Read_data[0] = AMC_Adress;
    Read_data[1] = 0x03;
    Read_data[2] = (First_Adress >> 8);
    Read_data[3] = First_Adress & 0xff;
    Read_data[4] = (Register_Number >> 8);
    Read_data[5] = Register_Number & 0xff;
    CRC_Check = AMC_CRC(Read_data, 6);
    Read_data[6] = CRC_Check & 0xff;
    Read_data[7] = (CRC_Check >> 8);

    i = U1_strSend(Read_data, 0x8);
    
    j = AMC_Receive(AMC_Receive_Data, (5 + Register_Number*2));   //receive the data form electric meter
    if((i==8) && (j==0)){
        return 1;
        puts(xtoa(j));
        puts("read error\n");
    }
    return 0;
}

/**
 * @brief 
 * This function is to write register data to electric meter
 * @param AMC_Adress                    the adress of AMC electric meter 
 * @param First_Adress                  the first adress of register that you want to write
 * @param Register_Number               the number of register you want to write
 * @param Register_Data                 the data you want to write
 * @return                              the flag data. 1 is successfully
 */
int AMC_Write_configuration(int AMC_Adress, int First_Adress, int Register_Number, char* Register_Data){
    char Write_Data[43];
    int CRC_Check = 0;
    char AMC_Receive_Data[8];
    int i, j, k;
    i = j = k = 0;

    Write_Data[0] = AMC_Adress;
    Write_Data[1] = 0x03;
    Write_Data[2] = (First_Adress >> 8);
    Write_Data[3] = First_Adress & 0xff;
    Write_Data[4] = (Register_Number >> 8);
    Write_Data[5] = Register_Number & 0xff;
    Write_Data[6] = Register_Number * 2;
    for (i = 0; i < (Register_Number * 2);i++){                 //write data to registers
        Write_Data[7 + i] = Register_Data[i];
    }
    CRC_Check = AMC_CRC(Write_Data, (7 + i));                   //write CRC check code
    Write_Data[7 + i] = CRC_Check & 0xff;
    Write_Data[8 + i] = (CRC_Check >> 8);

    j = U1_strSend(Write_Data, (9 + i));
    k = AMC_Receive(AMC_Receive_Data, 8);
    if((j==9+i) && (k==0))
        return 1;
    return 0;
}

/**
 * @brief 
 * This function is to process the data from AMC electric meter
 * @param AMC_Back_data                 the data from the sensor 
 * @param Data_len                      length of the data  
 * @param AMC_Upload_message            the data that was processed        
 * @return                              0 is successfully
 */
int AMC_data_proc(char* AMC_Back_data, int Data_len, char* AMC_Upload_message){
    int i = 0;
    int Check_Byte = 0;

    for (i = 0; i < Data_len; i++){
        AMC_Upload_message[7 + i] = AMC_Back_data[i];
    }
    
    for (i = 0; i < Data_len; i++){
        Check_Byte = AMC_Upload_message[5 + i] + Check_Byte;
    }
    AMC_Upload_message[4] = Data_len;
    AMC_Upload_message[5+Data_len] = Check_Byte;
    AMC_Upload_message[6+Data_len] = 0XEF;
    AMC_Upload_message[7+Data_len] = 0XEF;
    return 0;
}