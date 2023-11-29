#include "stm32f10x.h"                  // Device header
#include "./main.h"
#include "../User/bsp_gpio.h"
#include "../User/bsp_led.h"
#include "../User/bsp_TMC4671.h"
#include "../User/bsp_TurboCtrl.h"

void DISABLE_TMC(void)
{
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 1)
        ADCvolt[1] = 0;
}

void TurboCtrl(void)
{
    tmc4671_writeInt(0, 1, 0);
    if (tmc4671_readInt(1, 0, 0) == 0x34363731)
    {
        tmc4671_init_set();
        timeout = 100;
        while (timeout)
        {
            ClearWDG();
        }
        TMC4671_EN();
    }
    else
        TMC4671_DIS();
}
