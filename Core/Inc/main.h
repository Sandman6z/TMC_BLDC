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

extern unsigned int timeout;
extern unsigned char TIMEcount;
extern unsigned char ms_timeout;
extern uint8_t rtc_flag;
extern __IO uint16_t ADCConvertedValue[];
extern uint32_t ADCValue[];
extern uint32_t ADCVolt[];
extern uint32_t gBusPower, gMOSTemp, gBraking, gRES_status;
//extern int32_t gBusVoltage;

void NVIC_Configuration(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);
void TIM_Configuration(void);
void initBase(void);
void SetSysClockTo16(void);

#endif /*__MAIN_H__*/
