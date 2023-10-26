/**
 * @file main.c
 * @author Sandman Zhang (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <math.h>
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "../User/Global.h"
#include "../User/uart.h"
#include "../User/uart_process.h"
#include "../User/gpio.h"
#include "../User/TMC4671.h"
#include "../User/adc.h"
#include "../User/FAN_ctrl.h"

#define MAX_SPEED       60000
#define FAN_SPEED_MAX   20000
#define FAN_SPEED_MIN   4000

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration(void);
void SetSysClockTo16(void);
float calculate_temperature(uint32_t adc_V, float Bx);
void wdg_init(void);
void get_speed(void);

u8 rtc_flag;
uint8_t warkup_flag;
__IO uint16_t ADCConvertedValue[15];
int32_t Turbo_speed;
uint32_t ADCValue[15] = {0, 0, 0}; // 初始化前3个元素为0
uint32_t ADCvolt[15];
uint32_t POWER = 0, TEMSTATUS = 0, test = 0, RS = 0, RSTATUS = 0;
extern uint32_t FAN_SPEED_S, FAN_SPEED_M;

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

int main()
{	
    uint16_t ADC_count = 0, ADC_flag;

    SysInit();
    __set_PRIMASK(0);

    timeout = 100;
    while (timeout)
    {
        clrwdt();
    }
    init_TMC4671();
    tmc4671_writeInt(0, 1, 0);
    if (tmc4671_readInt(1, 0, 0) == 0x34363731)
    {
        tmc4671_init_set();
        timeout = 100;
        while (timeout)
        {
            clrwdt();
        }
        TMC4671_EN();
    }
    else
        TMC4671_DIS();

    timeout = 100;
    while (timeout)
    {
        clrwdt();
    }
    while (1)
    {
        clrwdt();
        if (ADC_count < 4)
        {
            ADC_count++;
            for (int i = 0; i < 6; i++)
            {
                ADCValue[i] = ADCValue[i] + ADCConvertedValue[i];
            }
            int t = 10;
            while (t)
            {
                t--;
                clrwdt();
            }
        }
        if (ADC_count >= 4)
        {
            ADC_count = 0;
            for (int i = 0; i < 6; i++)
            {
                ADCvolt[i]  = ADCValue[i] / 4;
                ADCvolt[i]  = ADCvolt[i] * 3300;
                ADCvolt[i]  = ADCvolt[i] >> 12;
                ADCValue[i] = 0;
            }
        }
   
        int VM     = (float)ADCvolt[3] * 6.77;	//	VM:Voltage of BUS
        int PWBUS  = (float)ADCvolt[4] * 6.77;	
        int VB     = (float)ADCvolt[0] * 6.77;	//	VB: Voltage of Brake
				float tem  = calculate_temperature(ADCvolt[2], 3490.0f) * 0.01f + tem * 0.99f;
        float tem2 = calculate_temperature(ADCvolt[2], 3020.0f) * 0.01f + tem2 * 0.99f;
//        float pwm  = 3 * tem - 130;
				
        if (tem < -40 || tem > 72)
            TEMSTATUS = 0;	                    //backup error
        else if (tem > 50)
            TEMSTATUS = 1;
        else
            TEMSTATUS = 1;
        if (PWBUS < 3500 && PWBUS > 1800)
            POWER = 1;
        else
            POWER = 0;

        if (VM > 3000)
        {
            LED_OV_ON;
            R_ON;
            RS = 1;
        }
        else
        {
            LED_OV_OFF;
            R_OFF;
            RS = 0;
        }
        if (RS == 0)
        {
            if (VB < 100)
                RSTATUS = 0;
            else
                RSTATUS = 1;
        }
        else
        {
            RSTATUS = 1;
        }
        if (POWER == 1 && TEMSTATUS == 1 && RSTATUS == 1)
            STAT_OUT_NORMAL;
        else
            STAT_OUT_ERROR;
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 1)
            ADCvolt[1] = 0;
				
				GPIOB->BSRR = GPIO_Pin_4;
				GPIOB->BRR = GPIO_Pin_4;
				
				GPIOB->ODR = 0X01;
				
        if (test >= 1 && test <= 1500 && POWER == 1 && TEMSTATUS == 1 && RSTATUS == 1)
        {
            TMC4671_EN();
            Turbo_speed = -(test * MAX_SPEED / 3000);
            tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000002);
            // Rotate right
            tmc4671_writeInt(0, TMC4671_PID_VELOCITY_TARGET, Turbo_speed);
            ADC_flag = 1;
        }
        else
        {
            if (ADC_flag)
            {
                ADC_flag = 0;
                tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000002);
                // Rotate right
                tmc4671_writeInt(0, TMC4671_PID_VELOCITY_TARGET, 0);
                TMC4671_DIS();
            }
        }
    }
}

/*********************************************************
 *��������: �ж�����
 *��    �룺��
 *��    ������
 *��    ��:
 *��������: 2011.07.05
 **********************************************************/
void NVIC_Configuration(void)
{
    //NVIC_InitTypeDef NVIC_InitStructure;
#ifdef VECT_TAB_RAM
    // Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else
    // Set the Vector Table base location at 0x08000000
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
    /* Configure the NVIC Preemption Priority Bits */
}
/*********************************************
 *��������: ʼ�ܶ˿�
 *��    �룺��
 *��    ������
 *��    ��:
 *��������: 2011.07.05
 *********************************************/
void RCC_Configuration(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);

    /*!< AFIO Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
}

void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // ��TIM2��ʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // ��TIM2��ʱ��

    TIM_DeInit(TIM2);
    TIM_InternalClockConfig(TIM2);
    TIM_TimeBaseStructure.TIM_Period = 9999;
    TIM_TimeBaseStructure.TIM_Prescaler = 55;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    // TIM_PrescalerConfig(TIM2, 36, TIM_PSCReloadMode_Immediate);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    // TIM_ARRPreloadConfig(TIM5, ENABLE);

    TIM_DeInit(TIM3);
    TIM_InternalClockConfig(TIM3);
    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_Prescaler = 55;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    // TIM_PrescalerConfig(TIM2, 36, TIM_PSCReloadMode_Immediate);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    /* TIM IT enable
       Enable TIM2 Update interrupt [TIM2����ж�����]*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);

    /*TIM2�ж�ʹ��*/
    //    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // �ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; // ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /*TIM3�ж�ʹ��*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; // �ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4; // ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void SetSysClockTo16(void)
{
    ErrorStatus HSEStartUpStatus;
    /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -------------------------*/
    /* RCC system reset(for debug purpose) */
    RCC_DeInit();
    /* Enable HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    /* Wait till HSE is ready */
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if (HSEStartUpStatus == SUCCESS)
    {
        /* Enable Prefetch Buffer */
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        /* Flash 2 wait state */
        FLASH_SetLatency(FLASH_Latency_2);
        /* HCLK = SYSCLK */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        /* PCLK2 = HCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1);
        /* PCLK1 = HCLK/2 */
        RCC_PCLK1Config(RCC_HCLK_Div2);

#ifdef STM32F10X_CL
        /* Configure PLLs *********************************************************/
        /* PLL2 configuration: PLL2CLK = (HSE / 5) * 8 = 40 MHz */
        RCC_PREDIV2Config(RCC_PREDIV2_Div5);
        RCC_PLL2Config(RCC_PLL2Mul_8);

        /* Enable PLL2 */
        RCC_PLL2Cmd(ENABLE);

        /* Wait till PLL2 is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLL2RDY) == RESET)
        {
        }

        /* PLL configuration: PLLCLK = (PLL2 / 5) * 7 = 56 MHz */
        RCC_PREDIV1Config(RCC_PREDIV1_Source_PLL2, RCC_PREDIV1_Div4);
        RCC_PLLConfig(RCC_PLLSource_PREDIV1, RCC_PLLMul_6);
#else
        /* PLLCLK = 8MHz * 2 = 16 MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_7);
#endif

        /* Enable PLL */
        RCC_PLLCmd(ENABLE);

        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
    else
    { /* If HSE fails to start-up, the application will have wrong clock configuration.
         User can add here some code to deal with this error */

        /* Go to infinite loop */
        while (1)
        {
            ;
        }
    }
}

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

float calculate_temperature(uint32_t adc_V, float Bx)
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
