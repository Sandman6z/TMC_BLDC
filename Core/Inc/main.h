#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include "../../User/bsp_adc.h"

#define     HEAD1               0xaa
#define     HEAD2               0x55
#define     HEAD3               0x00
#define     HEAD4               0x00
#define     END1                0xcc
#define     END2                0x33
#define     END3                0xc3
#define     END4                0x3c
#define     PRE_ADD             0x05
#define     BDU                 0x32

#define     ADC1_DR_Address     ((u32)0x40012400 + 0x4c)
#define     ClearWDG()          WWDG_SetCounter(0xff)

#define     ADC1_CH_NUM         8

#define     Turbo_Minspeed      3000
#define     Turbo_MAXspeed      60000

/*********���ڲ���************/
extern u8 nRec1, R_Data1, Rx_Buf1[15], RI1_flag;
extern u8 Tx_Buf2[30];
extern u8 nRec2, R_Data2, Rx_Buf2[15], RI2_flag;

extern unsigned int timeout;
extern unsigned char TIMEcount;
extern unsigned char ms_timeout;
extern unsigned char nRF_Sec;
extern unsigned char timeout_18B20;
extern unsigned char LED_stat;

extern uint8_t rtc_flag, warkup_flag;

extern __IO uint16_t ADCConvertedValue[];
extern uint32_t ADCValue[];
extern uint32_t ADCvolt[];
extern uint32_t POWER, TemStatus, Braking, Res_STATUS;
extern int32_t Voltage_BUS, targetValue;

void NVIC_Configuration(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);
void TIM_Configuration(void);
void Delay(u32 nCount);
void initBase(void);
void SetSysClockTo16(void);

#endif /*__MAIN_H__*/
