#ifndef __UART2_H__
#define __UART2_H__

#define U2_Re_Time_out  0xFFFF

unsigned char U2_flag_time_over;

/*********** Hardware addesses ***********/
#define U2_READ_REG             0x1f800020  //RX data read register
#define U2_BUSY_REG             0x1f800021  //Tx busy detect register
#define U2_WRITE_REG            0x1f800022  //Tx data write register
#define U2_CLRIRQ_REG           0x1f800023  //Write to clear irq flag
#define U2_CTL0_REG             0x1f800024  //Uart1/Lin(Local Interconnect Network) control register
#define U2_DATA_RDY_REG         0x1f800025  //Rx ready detect register

/*************** UART Setup***************/
void U2_Init();
char U2_Rec();
char U2_Send(char*);

#endif //__UART0_H__