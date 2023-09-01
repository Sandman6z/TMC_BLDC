#ifndef __GLOBAL_H
#define __GLOBAL_H

#include "stm32f10x.h"


//#include "spi_flash.h"

/*********串口参数************/
////////////////////////////////////////////////UART4 电源板
extern u8 nRec1,R_Data1,Rx_Buf1[15],RI1_flag;
/////////////////////////////////////////////////USART2 PC104
extern u8 Tx_Buf2[30];
extern u8 nRec2,R_Data2,Rx_Buf2[15],RI2_flag;


#define clrwdt() WWDG_SetCounter(0xff)

/**********定时器参数*************/
extern unsigned int timeout;
extern unsigned char TIMEcount;
extern unsigned char ms_timeout;
extern unsigned char nRF_Sec;
extern unsigned char timeout_18B20;
extern unsigned char LED_stat;
/***************报警音量*************/


void NVIC_Configuration(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);
void TIM_Configuration(void);
void Delay(u32 nCount);
void initBase(void);



extern u8 rtc_flag;

extern uint8_t warkup_flag;

extern u8 driver_trans;
 extern u16 step;
 extern u8 motor_flag;


extern  uint32_t Valve_time;
extern uint8_t Valve_flag; 
extern uint16_t Valve_count;
/////////////////////////////////////key
extern uint16_t Temperature_18B20;

extern u8 Beep_count;


extern CanTxMsg TxMessage;
extern CanRxMsg  RxMessage;


#define BEEP_TIME 2


#define HEAD1 0xaa
#define HEAD2 0x55
#define HEAD3 0x00
#define HEAD4 0x00
#define END1  0xcc
#define END2  0x33
#define END3  0xc3
#define END4  0x3c

#define PRE_ADD 0x05
#define BDU		0x32                //????μ￥?a


#define ADC1_DR_Address    ((u32)0x40012400+0x4c) 





#endif

