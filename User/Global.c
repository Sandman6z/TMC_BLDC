#include "./Global.h"




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

        usart2_send('S');
        usart2_send('E');
        usart2_send('=');
        for (int i = 3; i <= 7; i++)
        {
            usart2_send(Rx_Buf2[i]);
        }
        usart2_send(',');
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

void SysInit(void)
{
    SetSysClockTo16();
    RCC_Configuration();  // System Clocks Configuration
    NVIC_Configuration(); // �ж� NVIC configuration
    GPIO_Configuration(); // Configure the GPIO ports
    TIM_Configuration();
    InitUsart2();
    TMC4671_DIS();
    ADC1_MODE_CONFIG();
}

int MOS_TempCheck(void)
{
    if (tem < -40 || tem > 72)
        TEMSTATUS = 0;              // backup error
    else
        TEMSTATUS = 1;
}

int PowerCheck(void)
{
    if (PWBUS < 3500 && PWBUS > 1800)
        POWER = 1;
    else
        POWER = 0;
}

int Overvoltage_oprate(void)
{
    if (VM > 2800)
    {
        LED_OV_ON;
        R_ON;       //使能泄放电阻
        RS = 1;
    }
    else
    {
        LED_OV_OFF;
        R_OFF;
        RS = 0;
    }
    if (RS == 0 && VM < 100)
        RSTATUS = 0;
    else
        RSTATUS = 1;
}

/**
 * @brief 函数来进行逆向ADC值的映射
 * 
 * @param adc_value 
 *  原始数据范围：0~4095
 *  目标范围：		3000~42000
 * @return target_value 
 */
int inverseMapADCValue(int adc_value) 
{
    // 计算逆向缩放因子：原始范围宽度 / 目标范围宽度
    const double inverse_scale_factor = 4095.0 / (42000.0 - 3000.0);

    // 计算逆向偏移值：原始范围最小值 - (目标范围最小值 * 逆向缩放因子)
    const double inverse_offset = 0.0 - (3000.0 * inverse_scale_factor);

    // 使用逆向缩放因子和逆向偏移值将ADC值映射到目标值
    int target_value = (int)(adc_value / inverse_scale_factor + inverse_offset);
    
    // 确保目标值在合法范围内（3000到42000之间）
    if (target_value < 3000) 
    {
        target_value = 3000;
    } 
    else if (target_value > 42000) 
    {
        target_value = 42000;
    }
    
    return target_value;
}
