#define LED_MODULE_ENABLED
#ifdef LED_MODULE_ENABLED



#include "../Core/Inc/main.h"
#include "bsp_led.h"

void WorkStateIndicate(void)
{
    if (gBusPower == 1 && gMOSTemp == 1 && gRES_status == 1)
        WorkingLEDON;
    else
        WorkingLEDOFF;
}


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

#endif
