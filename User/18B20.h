/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __18B20_H
#define __18B20_H

#include "stm32f10x.h"

#define GPIO_18B20_DQ   GPIOA
#define Pin_18B20       GPIO_Pin_0

extern uint16_t Temperature_18B20;

void GPIO_18B20_init(void);
void WIRE_LOW_DQ(void);
void WIRE_HIGH_DQ(void);
void delay_18B20(uint16_t dt);
uint8_t RESET_18b20(void);
void WRITE_18b20(uint8_t c_Real);
uint16_t READ_18b20(void);
void DS18B20_process(void);
uint8_t Convert_18B20(void);
uint16_t Read_scratchpad(void);
uint16_t Read_Temp(void);
void init_18B20(void);

#endif
