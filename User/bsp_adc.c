#include "bsp_adc.h"
#include "../Core/Inc/main.h"
#include "bsp_TMC4671.h"

void ADC1_MODE_CONFIG(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,   ENABLE);    // Enable DMA clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	// Enable ADC1 and GPIOC clock
    /* DMA channel1 configuration */
    DMA_DeInit(DMA1_Channel1);
    DMA_InitStructure.DMA_PeripheralBaseAddr    = ADC1_DR_Address;     				// ADC��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr        = (u32)&ADCConvertedValue; 			// �ڴ��ַ
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize            = ADC1_CH_NUM;                           // 5;//ZHN20211123
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;        // �����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;             // �ڴ��ַ���̶�
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord;  // ����
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular; 				// ѭ������
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    /*����ADCʱ�ӣ�ΪPCLK2��4��Ƶ��*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    /* ADC1 configuration */
    ADC_DeInit(ADC1);                                                           // ��λADC1
    ADC_InitStructure.ADC_Mode                  = ADC_Mode_Independent;         // ����ADCģʽ
    ADC_InitStructure.ADC_ScanConvMode          = ENABLE;                       // ����ɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
    ADC_InitStructure.ADC_ContinuousConvMode    = ENABLE;                       // ��������ת��ģʽ������ͣ�ؽ���ADCת��
    ADC_InitStructure.ADC_ExternalTrigConv      = ADC_ExternalTrigConv_None;    // ��ʹ���ⲿ����ת��
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right;          // �ɼ������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel          = ADC1_CH_NUM;                       // 5;	 	//Ҫת����ͨ����Ŀ5//ZHN20211123
    ADC_Init(ADC1, &ADC_InitStructure);

    /**
     * @brief 
     * ADC_Channel_0    BRAKE_DETECT / Power Voltage      PA0            ADCvolt[0]
     * ADC_Channel_1        AD_SPEED / from BDU           PA1            ADCvolt[1]
     * ADC_Channel_2       MOS_T_ADC                      PA2            ADCvolt[6]
     * ADC_Channel_3       I_SUM_ADC                      PA3            ADCvolt[7]
     * ADC_Channel_4          VM_ADC / N.C.               PA4            ADCvolt[3]
     * ADC_Channel_5       TUR_T_ADC / N.C.               PA5            ADCvolt[2]
     * ADC_Channel_8           I_SET                      PB0            ADCvolt[4]
     * ADC_Channel_17       内部通道                       /                 /
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
    ADC_DMACmd(ADC1, ENABLE);                       // Enable ADC1 DMA
    ADC_Cmd(ADC1, ENABLE);                          // Enable ADC1
    ADC_ResetCalibration(ADC1);                     // ��λУ׼�Ĵ���
    while (ADC_GetResetCalibrationStatus(ADC1))
        ;                                           // �ȴ�У׼�Ĵ�����λ���
    ADC_StartCalibration(ADC1);                     // ADCУ׼
    while (ADC_GetCalibrationStatus(ADC1))
        ;                                           // �ȴ�У׼���
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);         // ����û�в����ⲿ����������ʹ����������ADCת��

    /* Enable DMA channel1 */
    DMA_Cmd(DMA1_Channel1, ENABLE);                 // ��ADC_SoftwareStartConvCmd(ADC1, ENABLE);��Ȼ���׳���ADC�ɼ����ִ�λ
}
