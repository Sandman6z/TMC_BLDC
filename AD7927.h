
#ifndef __AD7927_H
#define __AD7927_H
#endif

#include "stm32f10x.h"

void SPI_ad7927_Init(void);
void GPIO_ad7927_SPI_init(void);
u8 SPI_ad7927_SendByte(u8 byte);
void  delay_ad7927(uint16_t dt);
void init_ad7927(void);
void write_ad7927(uint16_t ch);
uint16_t read_ad7927(uint16_t ch);
uint16_t adc_7927(uint16_t ch);
//u16 SPI_ad7927_SendByte(u16 byte);
void write_ad7927(uint16_t ch);
uint16_t read_ad7927(uint16_t ch);



