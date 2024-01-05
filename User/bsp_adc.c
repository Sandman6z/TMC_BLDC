#include "bsp_adc.h"
#include "../Core/Inc/main.h"
#include "bsp_TMC4671.h"
#include "stm32f10x.h"

void ADC1_CONFIG(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr    = ADC1_DR_Address;
    DMA_InitStructure.DMA_MemoryBaseAddr        = (u32)&ADCConvertedValue;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize            = ADC1_CH_NUM;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular; 
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /*����ADCʱ�ӣ�ΪPCLK2��4��Ƶ��*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    /* ADC1 configuration */
    ADC_DeInit(ADC1);
    ADC_InitStructure.ADC_Mode                  = ADC_Mode_Independent;        
    ADC_InitStructure.ADC_ScanConvMode          = ENABLE;                      
    ADC_InitStructure.ADC_ContinuousConvMode    = ENABLE;                      
    ADC_InitStructure.ADC_ExternalTrigConv      = ADC_ExternalTrigConv_None;   
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;         
    ADC_InitStructure.ADC_NbrOfChannel          = ADC1_CH_NUM;                 
    ADC_Init(ADC1, &ADC_InitStructure);

/**
 * @brief 
 * ADC_Channel_0    BRAKE_DETECT / Power Voltage    PA0        ADCVolt[0]
 * ADC_Channel_1        AD_SPEED / from BDU         PA1        ADCVolt[1]
 * ADC_Channel_2       MOS_T_ADC                    PA2        ADCVolt[6]
 * ADC_Channel_3       I_SUM_ADC                    PA3        ADCVolt[7]
 * ADC_Channel_4          VM_ADC / N.C.             PA4        ADCVolt[3]
 * ADC_Channel_5       TUR_T_ADC / N.C.             PA5        ADCVolt[2]
 * ADC_Channel_8           I_SET                    PB0        ADCVolt[4]
 * ADC_Channel_17       内部通道                     /             /
 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 6, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 7, ADC_SampleTime_71Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 8, ADC_SampleTime_71Cycles5);     //after change remember to change the value of NUM_CH

    // Enable the temperature sensor and vref internal channel
    ADC_TempSensorVrefintCmd(ENABLE);
    ADC_DMACmd(ADC1, ENABLE);                  
    ADC_Cmd(ADC1, ENABLE);                     
    ADC_ResetCalibration(ADC1);                
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;                                      
    ADC_StartCalibration(ADC1);                
    while (ADC_GetCalibrationStatus(ADC1))
        ;                                      
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);    

    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);            
}

void ADCCalc(void)
{
    static uint16_t ADC_count = 0;
    static uint32_t ADCValue[ADC1_CH_NUM] = {0};
    
    if (ADC_count < 4)
    {
        ADC_count++;
        for (int i = 0; i < 6; i++) 
        {
            ADCValue[i] += ADCConvertedValue[i];
        }
    }
    if (ADC_count >= 4)
    {
        ADC_count = 0;
        for (int i = 0; i < 6; i++)
        {
            ADCVolt[i] = (ADCValue[i] * 825) >> 12;     //(ADCValue[i] *3300 / 4) >> 12;
            ADCValue[i] = 0;
        }
    }
}
