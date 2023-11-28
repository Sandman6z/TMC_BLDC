#include "./main.h"
#include "../User/bsp_gpio.h"
#include "../User/bsp_led.h"

void WorkStateIndicate(void)
{
    if (POWER == 1 && TEMSTATUS == 1 && RSTATUS == 1)
        STAT_OUT_NORMAL;
    else
        STAT_OUT_ERROR;
}
