/**
 * @file main.c
 * @author Sandman (zxxx@aeonmed.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-11
 * 
 * @copyright Copyright (c) 2023
 */

#include <math.h>
#include "../Inc/main.h"
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "../../User/bsp_adc.h"
#include "../../User/bsp_gpio.h"
#include "../../User/bsp_led.h"
#include "../../User/bsp_TMC4671.h"
#include "../../User/bsp_TurboCtrl.h"
#include "../../User/bsp_monitor.h"

uint8_t rtc_flag = 0;
__IO uint16_t ADCConvertedValue[ADC1_CH_NUM] = {0};
uint32_t ADCVolt[ADC1_CH_NUM] = {0};
/**
The table below gives the meaning of these variable: gBusPower, gMOSTemp, gBraking, gRES_status
  ========================================================+
      VALUE     |          0          |         1         |
  ========================================================+
    gBusPower   |        FAULT        |      NORMAL       |
  --------------------------------------------------------+
    gMOSTemp    |  MOS_Temp OVERHEAT  |  MOS_Temp NORMAL  |
  --------------------------------------------------------+
    gBraking    |       NORMAL        |   Overvoltaging   |
  --------------------------------------------------------+
   gRES_status  |    Break circuit    |      NORMAL       |
  ======================================================-=+
*/
uint32_t gBusPower = 0, gMOSTemp = 0, gBraking = 0, gRES_status = 1;
int32_t gTargetValue = 0;
uint16_t turboRunning = 0;

int main()
{
    SysInit();
    __set_PRIMASK(0);

    timeout = 100;
    while (timeout)
    {
        ClearWDG();
    }
    init_TMC4671();
    TurboCtrl();

    timeout = 100;
    while (timeout)
    {
        ClearWDG();
    }
    while (1)
    {
        ClearWDG();
        ADCCalc();
        BUSVoltageCalc();
        PowerCheck();
        OverVoltageOprate();
        MOS_TempCheck();
        ResExistDetect();
        WorkStateIndicate();
        gTargetValue = inverseMapADCValue(ADCVolt[1]);                           //get DAC value from BDU control board
        if (gTargetValue >= Turbo_Minspeed && gTargetValue <= Turbo_MAXspeed )  //&& gBusPower == 1 && gMOSTemp == 1 && gRES_status == 1
        {
            ClearWDG();
            TMC4671_EN();
            TMC4671_RUN_Mode;
            tmc4671_writeInt(0, TMC4671_PID_VELOCITY_TARGET, -gTargetValue);
            turboRunning = 1;
        }
        else
        {
            if (turboRunning)
            {
                turboRunning = 0;
                ClearWDG();
                TMC4671_RUN_Mode;
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
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

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

    /*TIM2�ж�ʹ��*/
    //    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; // �ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; // ���ȼ�
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
