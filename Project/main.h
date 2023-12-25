#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include "../User/bsp_adc.h"

#define BEEP_TIME 2

#define HEAD1 0xaa
#define HEAD2 0x55
#define HEAD3 0x00
#define HEAD4 0x00
#define END1 0xcc
#define END2 0x33
#define END3 0xc3
#define END4 0x3c

#define PRE_ADD 0x05
#define BDU     0x32 //????�̣�?a

#define ADC1_DR_Address ((u32)0x40012400 + 0x4c)
#define ClearWDG() WWDG_SetCounter(0xff)

/*********���ڲ���************/
extern u8 nRec1, R_Data1, Rx_Buf1[15], RI1_flag;
extern u8 Tx_Buf2[30];
extern u8 nRec2, R_Data2, Rx_Buf2[15], RI2_flag;

/**********��ʱ������*************/
extern unsigned int timeout;
extern unsigned char TIMEcount;
extern unsigned char ms_timeout;
extern unsigned char nRF_Sec;
extern unsigned char timeout_18B20;
extern unsigned char LED_stat;

extern uint8_t rtc_flag, warkup_flag;
extern u8 driver_trans;
extern u16 step;
extern u8 motor_flag;
extern uint32_t Valve_time;
extern uint8_t Valve_flag;
extern uint16_t Valve_count;
extern uint16_t Temperature_18B20;
extern u8 Beep_count;
//extern CanTxMsg TxMessage;
//extern CanRxMsg RxMessage;
extern __IO uint16_t ADCConvertedValue[];
extern uint32_t ADCValue[]; // 初始化前3个元素为0
extern uint32_t ADCvolt[];
extern uint32_t POWER, TEMSTATUS, RS, RSTATUS;
extern int32_t Voltage_BUS, targetValue;
extern float tem, tem2;

void NVIC_Configuration(void);
void GPIO_Configuration(void);
void RCC_Configuration(void);
void TIM_Configuration(void);
void Delay(u32 nCount);
void initBase(void);

void NVIC_Configuration(void);
void RCC_Configuration(void);
void TIM_Configuration(void);
void SetSysClockTo16(void);
void GPIO_Configuration(void);
void wdg_init(void);
void get_speed(void);
float Calculate_temperature(uint32_t adc_V, float Bx);
void SysInit(void);
void MOS_TempCheck(void);
void PowerCheck(void);
void Overvoltage_oprate(void);
int inverseMapADCValue(uint16_t adc_value);


#endif /*__MAIN_H__*/
