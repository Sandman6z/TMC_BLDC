#include <math.h>
#include "../Core/Inc/main.h"
#include "../User/bsp_adc.h"
#include "../User/bsp_gpio.h"
#include "../User/bsp_monitor.h"

int32_t Voltage_BUS =0;

void wdg_init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
    WWDG_SetPrescaler(WWDG_Prescaler_8);
    WWDG_SetWindowValue(0x7f);
    WWDG_SetCounter(0x7f);
    WWDG_Enable(0x7f);
}

void get_speed(void)
{
    uint32_t temp;
    if (RI2_flag)
    {
        temp =        ((uint32_t)Rx_Buf2[3] - 0x30) * 10000;
        temp = temp + ((uint32_t)Rx_Buf2[4] - 0x30) * 1000;
        temp = temp + ((uint32_t)Rx_Buf2[5] - 0x30) * 100;
        temp = temp + ((uint32_t)Rx_Buf2[6] - 0x30) * 10;
        temp = temp + ((uint32_t)Rx_Buf2[7] - 0x30);
        RI2_flag = 0;
        Rx_Buf2[9] = '\0';
    }
}

float Calculate_temperature(uint32_t adc_V, float Bx)
{
    float Rt = 0, VCC = 0, temp = 0;
    float Rp = 10000;
    float Ka = 273.15;
    float T2 = Ka + 25;

    VCC = (4096 * 1200) / ADCConvertedValue[5];
    Rt = (Rp * adc_V) / (VCC - adc_V);
    temp = 1.0f / ((1.0f / T2) + (log(Rt / Rp) / Bx)) - Ka + 0.5f;

    return temp;
}

void MOS_TempCheck(void)
{
    float tem = 0.0f;
    tem = Calculate_temperature(ADCVolt[2], 3490.0f) * 0.01f + tem * 0.99f;
    
    if (tem < -40 || tem > 72)
        TemStatus = 0;              // backup error
    else
        TemStatus = 1;
}

void BUS_Voltage_Calc(void)
{
    Voltage_BUS = (float)ADCVolt[0] * 6.77;            //Voltage_BUS:Voltage of BUS
}

void PowerCheck(void)
{
    if (Voltage_BUS > 1800 && Voltage_BUS < 2800)
        POWER = 1;
    else
        POWER = 0;
}

void Overvoltage_oprate(void)
{
    if (Voltage_BUS > 2800)
    {
        LED_OV_ON;
        BrakeRes_ON;       //使能泄放电阻
        Braking = 1;
    }
    else
    {
        LED_OV_OFF;
        BrakeRes_OFF;
        Braking = 0;
    }
}

void ResExistDetect(void)
{
    if (Braking == 0 && Voltage_BUS < 100)
        Res_STATUS = 0;
    else
        Res_STATUS = 1;     //think whether left this sentence
}

void SysInit(void)
{
    SetSysClockTo16();
    RCC_Configuration();
    NVIC_Configuration();
    GPIO_Configuration();
    TIM_Configuration();
    TMC4671_DIS();
    ADC1_MODE_CONFIG();
}

/**
 * @brief 函数来进行逆向ADC值的映射
 * @param adc_value 
 * @return target_value 
 */
int inverseMapADCValue(uint16_t adc_value) 
{
    float target_value;
    target_value = (float)(adc_value * 19.38 - 326.8);
    
    // 确保目标值在合法范围内（3000到42000之间）
    if (target_value < Turbo_Minspeed) 
    {
        target_value = Turbo_Minspeed;
    } 
    else if (target_value > Turbo_MAXspeed) 
    {
        target_value = Turbo_MAXspeed;
    }
    
    return target_value;
}
