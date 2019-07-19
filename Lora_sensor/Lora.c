/*--------------------------------------------------------------------
 * TITLE: Lora command function
 * AUTHOR: haikang
 * DATE CREATED: 2019/7/16
 * FILENAME: Lora.c
 * PROJECT: IOT_Monitoring_System
 * DESCRIPTION: 
 *--------------------------------------------------------------------*/

#include"Lora.h"
#include"str.h"


/**
 * @brief 
 * This function is to read local configuration of the Lora
 * @param Local_Configuration_Return     the configuration was returned from the Lora
 * @return                               the flag that Whether the function is successfully executed. 1 is successfully
 */
int Read_Local_Configuration(char* Local_Configuration_Return){
    char Local_Configuration_data[6] = {0xAB, 0xBC, 0xCD, 0xD1, 0xAA};
    U2_strSend(Local_Configuration_data, 5);
    int result = U2_strRec(Local_Configuration_Return, 76);

    if(result==76){
        puts(xtoa(result));
        puts("got the Local_Configuration\n");
        return 1;
    }
    puts(xtoa(result));
    puts("error\n");
    return 0;
} 

/**
 * @brief 
 * This function is to read remote configuration of the Lora
 * @param Remote_Configuration_Return   the configuration was returned from the Lora
 * @param adress_high                   the high byte adress of remote lora module that you want to read    
 * @param adress_low                    the low byte adress of remote lora module that you want to read          
 * @return                              the flag that Whether the function is successfully executed. 1 is successfully
 */
int Read_Remote_Configuration(char* Remote_Configuration_Return, int adress_high, int adress_low){
    char Remote_Configuration_data[7] = {0xAB, 0XBC, 0XCD, 0XD2, adress_high, adress_low, 0XAA};
    U2_strSend(Remote_Configuration_data, 7);
    int result = U2_strRec(Remote_Configuration_Return, 77);
    
    if(result == 77){
        puts("got the Remote_Configuration\n");
        return 1;
    }
    return 0;
}

/**
 * @brief 
 * This function is to modify configuration of the Lora
 * @param Modify_Configuration_Return   the reply message was returned from the Lora
 * @param adress_high                   the high byte adress of lora module that you want to modify   
 * @param adress_low                    the low byte adress of lora module that you want to modify          
 * @return                              the flag that Whether the function is successfully executed. 1 is successfully
 */
int Modify_Configuration(char* Modify_Configuration_Return, int adress_high, int adress_low, char* New_Configuration_data){
    char Modify_Configuration_data[74];
    Modify_Configuration_data[0] = 0xAB;
    Modify_Configuration_data[1] = 0xBC;
    Modify_Configuration_data[2] = 0xCD;
    Modify_Configuration_data[3] = 0xD3;
    Modify_Configuration_data[4] = adress_high;
    Modify_Configuration_data[5] = adress_low;
    for (int i = 0; i < 67;i++){
        Modify_Configuration_data[6 + i] = New_Configuration_data[i];
    }
    Modify_Configuration_data[73] = 0xAA;       //this message need to add very long configuration data 
    U2_strSend(Modify_Configuration_data, 74);
    int result = U2_strRec(Modify_Configuration_Return, 7);
    
    if(result == 7){
        puts("Configuration have been modified\n");
        return 1; 
    }
    return 0;
    
}
 
int Module_Search(char* Module_Search_Return){}

/**
 * @brief 
 * This function is to reset the Lora
 * @param Module_Reset_Return           the reply message was returned from the Lora
 * @param adress_high                   the high byte adress of lora module that you want to reset   
 * @param adress_low                    the low byte adress of lora module that you want to reset          
 * @return                              the flag that Whether the function is successfully executed. 1 is successfully
 */
int Module_Reset(char* Module_Reset_Return, int adress_high, int adress_low){
    char Module_Reset_data[7] = {0xAB, 0XBC, 0XCD, 0XD6, adress_high, adress_low, 0XAA};
    U2_strSend(Module_Reset_data, 7);
    int result = U2_strRec(Module_Reset_Return, 7);
    
    if(result == 7){
        puts("Module have been reseted successfully\n");
        return 1;
    }
    return 0;
}

/**
 * @brief 
 * This function is to add the destination address in THE front of the instruction
 * @param Destination_Address_add_Return    the reply message was returned from the Lora
 * @param adress_high                       the high byte destination adress of lora module that you want to add   
 * @param adress_low                        the low byte destination adress of lora module that you want to add    
 * @param Enable_setting                    1 is enable, 0 is disable      
 * @return                                  the flag that Whether the function is successfully executed. 1 is successfully
 */
int Destination_Address_add(char* Destination_Address_add_Return, int adress_high, int adress_low, int Enable_setting){
    char Destination_Address_add_data[9] = {0xAB, 0XBC, 0XCD, 0XD9, adress_high, adress_low, 0x01, Enable_setting, 0XAA};
    U2_strSend(Destination_Address_add_data, 9);
    int result = U2_strRec(Destination_Address_add_Return, 8);
    
    if((result == 8) && (Enable_setting==Destination_Address_add_Return[6])){
        puts("Destination_Address_add\n");
        return 1;
    }
    return 0;

}

/**
 * @brief 
 * This function is to add whitelist of the bridging module
 * @param Bridging_Module_Whitelist_Return    the reply message was returned from the Lora
 * @param adress_high                         the high byte adress of lora module that you want to add   
 * @param adress_low                          the low byte adress of lora module that you want to add       
 * @return                                    the flag that Whether the function is successfully executed. 1 is successfully
 */
int Bridging_Module_Whitelist(char* Bridging_Module_Whitelist_Return, int adress_high, int adress_low, int Whitelist_adress_high, int Whitelist_adress_low){
    char Bridging_Module_Whitelist_data[10] = {0xAB, 0XBC, 0XCD, 0XDC, adress_high, adress_low, 0X01, Whitelist_adress_high, Whitelist_adress_low, 0XAA};
    U2_strSend(Bridging_Module_Whitelist_data, 10);
    int result = U2_strRec(Bridging_Module_Whitelist_Return, 7);
    
    if(result == 7){
        puts("Whitelist have been setted successfully\n");
        return 1;
    }
    return 0;
}

/**
 * @brief 
 * This function is to send the data to the lora
 * @param Lora_data                     the data that you want send to the lora
 * @param data_len                      length of the data   
 * @param Destination_adress_high       the high byte destination adress of lora module that you want to send
 * @param Destination_adress_low        the low byte destination adress of lora module that you want to send      
 * @return                              the flag that Whether the function is successfully executed. 1 is successfully
 */
int Lora_Data_Send(char *Lora_data, int data_len, int Destination_adress_high, int Destination_adress_low){
    char Send_Data[1000];
    int i = 0;
    int send_back = 0;
    Send_Data[0] = Destination_adress_high;
    Send_Data[1] = Destination_adress_low;
    for (i = 0; i < data_len;i++){          //destination adress need to add in the front of the data
        Send_Data[2 + i] = Lora_data[i];
    }
    Send_Data[2 + i] = '\n';
    data_len = data_len + 2;
    send_back=U2_strSend(Send_Data, data_len);
    if(send_back == data_len){
        puts("successfully send!\n");
        return 1;
    }
    return 0;
}

/**
 * @brief 
 * This function is to test the lora  
 * @return                              the flag that Whether the function is successfully executed. 1 is successfully
 */
int Lora_test(){
    char Send_Data_test[10]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};
    int send_back = 0;
    send_back=U2_strSend(Send_Data_test, 10);
    if(send_back==10){
        puts("OK\n");
        return 1;
    }
    return 0;
}

/**
 * @brief 
 * This function is to receive the data from the other lora
 * @param Receive_data                  the data that you received from the other lora
 * @param Receive_data_len              length of the data       
 * @return                              the flag that Whether the function is successfully executed. 1 is successfully
 */
int Lora_Data_Receive(char* Receive_data, int Receive_data_len){
    int data_len = U2_strRec(Receive_data, Receive_data_len);
    if(data_len == Receive_data_len){
        puts("Lora successfully receive\n");
        return 1;
    }
    return 0;
}