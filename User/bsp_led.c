#include "./main.h"
#include "../User/bsp_gpio.h"
#include "../User/bsp_led.h"

GPIO_TypeDef* GPIO_PORT[LEDn] = {LED_OV_GPIO_PORT, LED_ERROR_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED_OV_PIN, LED_ERROR_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED_OV_GPIO_CLK, LED_ERROR_GPIO_CLK};


void BSP_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Enable the GPIO_LED Clock */
  RCC_APB2PeriphClockCmd(GPIO_CLK[Led], ENABLE);

  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;

  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}

void LED_Configuration(void)
{
    BSP_LEDInit(LED_OV);
    BSP_LEDInit(LED_ERROR);
}

void WorkStateIndicate(void)
{
    if (POWER == 1 && TEMSTATUS == 1 && Res_STATUS == 1)
        STAT_OUT_NORMAL;
    else
        STAT_OUT_ERROR;
}
