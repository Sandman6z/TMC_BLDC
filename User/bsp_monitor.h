#ifndef __BSP_MONITOR_H__
#define __BSP_MONITOR_H__

#include "stm32f10x.h"                  // Device header

void wdg_init(void);
void BUSVoltageCalc(void);
void PowerCheck(void);
void OverVoltageOprate(void);
void ResExistDetect(void);
float Calculate_temperature(uint32_t adc_V, float Bx);
void MOS_TempCheck(void);
void SysInit(void);
int inverseMapADCValue(uint16_t adc_value);

#endif /* __BSP_MONITOR_H__ */
