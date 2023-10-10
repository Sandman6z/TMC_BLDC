#ifndef __GPIO_H__
#define __GPIO_H__

// #define GPIO_IO0_TMC4671 GPIOA
// #define IO0_TMC4671_Pin GPIO_Pin_12

// #define GPIO_IO1_TMC4671 GPIOA
// #define IO1_TMC4671_Pin GPIO_Pin_11

#define TMC4671_EN()    	GPIO_SetBits(GPIO_ENIN_TMC4671, ENIN_TMC4671_Pin)
#define TMC4671_DIS()   	GPIO_ResetBits(GPIO_ENIN_TMC4671, ENIN_TMC4671_Pin)

#define LED_OV_OFF  		GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED_OV_ON   		GPIO_ResetBits(GPIOB, GPIO_Pin_5)

#define R_ON    			GPIO_SetBits(GPIOB, GPIO_Pin_4)
#define R_OFF   			GPIO_ResetBits(GPIOB, GPIO_Pin_4)

#define LED_ERROR_OFF   	GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LED_ERROR_ON    	GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define STAT_OUT_NORMAL 	GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define STAT_OUT_ERROR  	GPIO_ResetBits(GPIOB, GPIO_Pin_7)

#define GPIO_ENIN_TMC4671   GPIOC
#define ENIN_TMC4671_Pin    GPIO_Pin_15

#define GPIO_ENOUT_TMC4671  GPIOC
#define ENOUT_TMC4671_Pin   GPIO_Pin_14

#define GPIO_STATUS_TMC4671 GPIOB
#define STATUS_TMC4671_Pin  GPIO_Pin_1


void GPIO_Configuration(void);

#endif /* __GPIO_H__ */
