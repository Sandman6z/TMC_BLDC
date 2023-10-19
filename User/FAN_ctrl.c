#include "../User/FAN_ctrl.h"
#include "stm32f10x.h"

//void Valva_Init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//    TIM_OCInitTypeDef TIM_OCInitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;

//    TIM_TimeBaseInitTypeDef TimeBaseInitStruct;
//    //	DMA_InitTypeDef DMA_InitStructure;

//    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
//    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_9;
//    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;
//    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_12;
//    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); // ???aTIM2??????????

//    TIM_Cmd(TIM4, DISABLE);
//    TIM_DeInit(TIM4);
//    TIM_InternalClockConfig(TIM4);

//    /* ---------------------------------------------------------------
//    TIM2 Configuration: Output Compare Toggle Mode:
//    --------------------------------------------------------------- */
//    /* Time base configuration */
//    TIM_TimeBaseStructure.TIM_Period        = 1119;
//    TIM_TimeBaseStructure.TIM_Prescaler     = 1;
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

//    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

//    /* Output Compare Toggle Mode configuration: Channel4 */
//    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM1;
//    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    TIM_OCInitStructure.TIM_Pulse       = 0;
//    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_High;
//    TIM_OC4Init(TIM4, &TIM_OCInitStructure);

//    TIM_Cmd(TIM4, ENABLE);

//    NVIC_InitStructure.NVIC_IRQChannel                      = TIM4_IRQn; // ??????
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 5; // ?????
//    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);

//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // ????TIM1?????????

//    TIM_DeInit(TIM1);
//    TimeBaseInitStruct.TIM_Period           = 0xFFFF;
//    TimeBaseInitStruct.TIM_Prescaler        = 0x00;
//    TimeBaseInitStruct.TIM_ClockDivision    = TIM_CKD_DIV1;
//    TimeBaseInitStruct.TIM_CounterMode      = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM1, &TimeBaseInitStruct);

//    TIM_TIxExternalClockConfig(TIM1, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising, 0);
//    TIM_ETRClockMode2Config(TIM1, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);
//    TIM_SetCounter(TIM1, 0);
//    TIM_Cmd(TIM1, ENABLE);

//    NVIC_InitStructure.NVIC_IRQChannel                      = TIM1_UP_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0; // ????????0
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 6;
//    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//}

//void Valva1_pwm(uint16_t dt) // FAN
//{
//    //	TIM_OCInitTypeDef  TIM_OCInitStructure;
//    //	/* Output Compare Toggle Mode configuration: Channel4 */
//    //  TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;
//    //  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//    //  TIM_OCInitStructure.TIM_Pulse = dt;
//    //  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//    //  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
//    if (dt > 100)
//        dt = 100;
//    if (dt < 0)
//        dt = 0;
//    TIM_SetCompare4(TIM4, (1119 * dt) / 100);
//}
