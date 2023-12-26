#ifndef __GPIO_H__
#define __GPIO_H__

#define GPIO_ENIN_TMC4671   GPIOC
#define ENIN_TMC4671_Pin    GPIO_Pin_15

#define GPIO_ENOUT_TMC4671  GPIOC
#define ENOUT_TMC4671_Pin   GPIO_Pin_14

#define GPIO_STATUS_TMC4671 GPIOB
#define STATUS_TMC4671_Pin  GPIO_Pin_1

#define TMC4671_EN()    	GPIO_SetBits(GPIO_ENIN_TMC4671, ENIN_TMC4671_Pin)
#define TMC4671_DIS()   	GPIO_ResetBits(GPIO_ENIN_TMC4671, ENIN_TMC4671_Pin)

#define LED_OV_OFF  		GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED_OV_ON   		GPIO_ResetBits(GPIOB, GPIO_Pin_5)

#define BrakeRes_ON    		GPIO_SetBits(GPIOB, GPIO_Pin_4)
#define BrakeRes_OFF   		GPIO_ResetBits(GPIOB, GPIO_Pin_4)

#define LED_ERROR_OFF   	GPIO_SetBits(GPIOB, GPIO_Pin_6)
#define LED_ERROR_ON    	GPIO_ResetBits(GPIOB, GPIO_Pin_6)

#define STAT_OUT_NORMAL 	GPIO_SetBits(GPIOB, GPIO_Pin_7)
#define STAT_OUT_ERROR  	GPIO_ResetBits(GPIOB, GPIO_Pin_7)


void GPIO_Configuration(void);

#endif /* __GPIO_H__ */
