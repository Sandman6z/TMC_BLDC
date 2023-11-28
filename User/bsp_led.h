#ifndef __BSP_LED_H__
#define __BSP_LED_H__


#define LEDn                             2

#define LED_OV_PIN                         GPIO_Pin_5
#define LED_OV_GPIO_PORT                   GPIOB
#define LED_OV_GPIO_CLK                    RCC_APB2Periph_GPIOB  
  
#define LED_ERROR_PIN                      GPIO_Pin_6
#define LED_ERROR_GPIO_PORT                GPIOB
#define LED_ERROR_GPIO_CLK                 RCC_APB2Periph_GPIOB  


typedef enum 
{
  LED_OV    = 0,
  LED_ERROR = 1,
} Led_TypeDef;


void LED_Configuration(void);
void WorkStateIndicate(void);

#endif /* __BSP_LED_H__ */
