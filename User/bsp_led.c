#include "../Core/Inc/main.h"
#include "bsp_led.h"

void WorkStateIndicate(void)
{
    if (POWER == 1 && TemStatus == 1 && Res_STATUS == 1)
        STAT_OUT_NORMAL;
    else
        STAT_OUT_ERROR;
}


//PB7 LED

void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(LED1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin     = LED1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz; 
    GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(LED1_GPIO_PORT, LED1_GPIO_PIN);
}

void LED_Running(void)
{
    LED1_ON;
    Delay_ms(200);
    LED1_OFF;
    Delay_ms(200);
}
