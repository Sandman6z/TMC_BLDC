#include <math.h>
#include "../Core/Inc/main.h"
#include "../User/bsp_adc.h"
#include "../User/bsp_gpio.h"
#include "../User/bsp_led.h"
#include "../User/bsp_monitor.h"

int32_t gBusVoltage = 0;

void wdg_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(0x7f);
    WWDG_SetCounter(0x7f);
    WWDG_Enable(0x7f);
}

void BUSVoltageCalc(void)
{
    gBusVoltage = (float)ADCVolt[0] * 6.77;            //gBusVoltage:Voltage of BUS
}

void PowerCheck(void)
{
    if (gBusVoltage > 1800 && gBusVoltage < 2800)
        gBusPower = 1;
    else
        gBusPower = 1;  //for debug Turbo stop.
}

void OverVoltageOprate(void)
{
    if (gBusVoltage > 2800)
    {
        LED_OV_ON;
        BrakeRes_ON;       //使能泄放电阻
        gBraking = 1;
    }
    else
    {
        LED_OV_OFF;
        BrakeRes_OFF;
        gBraking = 0;
    }
}

void ResExistDetect(void)
{
    if (gBraking == 0 && gBusVoltage < 100)
        gRES_status = 0;
    else
        gRES_status = 1;     //think whether left this sentence
}

float Calculate_temperature(uint32_t adc_V, float Bx)
{
    float Rt = 0, VCC = 0, temp = 0;
    float Rp = 10000;
    float Ka = 273.15;
    float T2 = Ka + 25;

    VCC = (4096 * 1200) / ADCConvertedValue[2];
    Rt = (Rp * adc_V) / (VCC - adc_V);
    temp = 1.0f / ((1.0f / T2) + (log(Rt / Rp) / Bx)) - Ka + 0.5f;

    return temp;
}

void MOS_TempCheck(void)
{
    float temp = Calculate_temperature(ADCVolt[2], 3490.0f) * 0.01f + temp * 0.99f;
    
    if (temp < -40 || temp > 72)
        gMOSTemp = 0;              // backup error
    else
        gMOSTemp = 1;
}

void SysInit(void)
{
    SetSysClockTo16();
    RCC_Configuration();
    NVIC_Configuration();
    GPIO_Configuration();
    TIM_Configuration();
    TMC4671_DIS();
    ADC1_CONFIG();
}

/**
 * @brief 函数来进行逆向ADC值的映射
 * @param adcValue 
 * @return targetValue 
 */
int inverseMapADCValue(uint16_t adcValue) 
{
    float targetValue = adcValue * 20;
    
    // 确保目标值在合法范围内
    if (targetValue < Turbo_Minspeed) 
    {
        targetValue = Turbo_Minspeed;
    } 
    else if (targetValue > Turbo_MAXspeed) 
    {
        targetValue = Turbo_MAXspeed;
    }
    
    return targetValue;
}
