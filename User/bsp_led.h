#ifndef __BSP_LED_H__
#define __BSP_LED_H__

#include "stm32f10x.h"

#define BrakeRes_ON    		GPIO_SetBits(GPIOB, GPIO_Pin_4)
#define BrakeRes_OFF   		GPIO_ResetBits(GPIOB, GPIO_Pin_4)

#define LED_OV_OFF  		GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED_OV_ON   		GPIO_ResetBits(GPIOB, GPIO_Pin_5)

#define LED_ERROR_OFF   	GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LED_ERROR_ON    	GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define STAT_OUT_NORMAL 	GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define STAT_OUT_ERROR  	GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define LED1_GPIO_PORT      GPIOB
#define LED1_GPIO_CLK       RCC_APB2Periph_GPIOB
#define LED1_GPIO_PIN       GPIO_Pin_7


#define digitalHi(p, i)     {p -> BSRR = i;}
#define digitalLo(p, i)     {p -> BRR = i;}
#define digitalToggle(p, i) {p -> ODR ^= i;}

#define LED1_TOGGLE         digitalToggle(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_OFF	        digitalHi(LED1_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_ON		        digitalLo(LED1_GPIO_PORT, LED1_GPIO_PIN)
               

void LED_GPIO_Config(void);
void WorkStateIndicate(void);
void LED_Running(void);

#endif /* __BSP_LED_H__ */
