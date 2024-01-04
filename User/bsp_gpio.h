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

void GPIO_Configuration(void);

#endif /* __GPIO_H__ */
