#ifndef __ADC_H__
#define __ADC_H__

#include "stm32f10x.h"

//extern __IO uint16_t ADCConvertedValue[10];

void ADC1_CONFIG(void);
void ADCInit(void);
void ADCCalc(void);

#endif /* __ADC_H__ */
