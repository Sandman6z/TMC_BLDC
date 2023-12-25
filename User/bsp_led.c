#include "../Core/Inc/main.h"
#include "bsp_gpio.h"
#include "bsp_led.h"

void WorkStateIndicate(void)
{
    if (POWER == 1 && TEMSTATUS == 1 && Res_STATUS == 1)
        STAT_OUT_NORMAL;
    else
        STAT_OUT_ERROR;
}
