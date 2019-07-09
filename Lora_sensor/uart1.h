#ifndef __UART1_H__
#define __UART1_H__

#define U1_Re_Time_out  0xFFFF

unsigned char U1_flag_time_over;

/*********** Hardware addesses ***********/
#define U1_READ_REG             0x1f800010  //RX data read register
#define U1_BUSY_REG             0x1f800011  //Tx busy detect register
#define U1_WRITE_REG            0x1f800012  //Tx data write register
#define U1_CLRIRQ_REG           0x1f800013  //Write to clear irq flag
#define U1_CTL0_REG             0x1f800014  //Uart1/Lin(Local Interconnect Network) control register
#define U1_DATA_RDY_REG         0x1f800015  //Rx ready detect register

/*************** UART Setup***************/
void U1_Init();
char U1_Rec();
char U1_Send(char*);

#endif //__UART0_H__
