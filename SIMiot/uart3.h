#ifndef __UART3_H__
#define __UART3_H__

#define U3_Re_Time_out  0xFFFF

unsigned char U3_flag_time_over;

/*********** Hardware addesses ***********/
#define U3_READ_REG             0x1f800030  //RX data read register
#define U3_BUSY_REG             0x1f800031  //Tx busy detect register
#define U3_WRITE_REG            0x1f800032  //Tx data write register
#define U3_CLRIRQ_REG           0x1f800033  //Write to clear irq flag
#define U3_CTL0_REG             0x1f800034  //Uart1/Lin(Local Interconnect Network) control register
#define U3_DATA_RDY_REG         0x1f800035  //Rx ready detect register

/*************** UART Setup***************/
void U3_Init();
char U3_Rec();
char U3_Send(char*);

#endif //__UART0_H__