/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#include <stdio.h>


#include "stm32f10x_it.h"
#include  "Global.h"
#include "uart.h"
#include "uart_process.h"



#include "math.h"

#include"TMC4671.h"



void initBase(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void TIM_Configuration1(void);
void SetSysClockTo16(void);

float temp_data(uint32_t adc_V);
float temp_data2(uint32_t adc_V);

void get_sudu(void);
void adcinit(void);
void adc1_mode_config(void);
 void Valva_Init(void);
 void Valva1_pwm(uint16_t dt);
int32_t speed_v;



#define GPIO_ENIN_TMC4671 GPIOC
#define ENIN_TMC4671_Pin GPIO_Pin_15

#define GPIO_ENOUT_TMC4671 GPIOC
#define ENOUT_TMC4671_Pin GPIO_Pin_14

#define GPIO_STATUS_TMC4671 GPIOB
#define STATUS_TMC4671_Pin GPIO_Pin_1

//#define GPIO_IO0_TMC4671 GPIOA
//#define IO0_TMC4671_Pin GPIO_Pin_12

//#define GPIO_IO1_TMC4671 GPIOA
//#define IO1_TMC4671_Pin GPIO_Pin_11

#define TMC4671_EN() GPIO_SetBits(GPIO_ENIN_TMC4671,ENIN_TMC4671_Pin)
#define TMC4671_DIS() GPIO_ResetBits(GPIO_ENIN_TMC4671,ENIN_TMC4671_Pin)

#define LED_OV_OFF     GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define LED_OV_ON      GPIO_ResetBits(GPIOB,GPIO_Pin_5)

#define R_ON       GPIO_SetBits(GPIOB,GPIO_Pin_4)
#define R_OFF      GPIO_ResetBits(GPIOB,GPIO_Pin_4)

#define LED_ERROR_OFF       GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define LED_ERROR_ON      GPIO_ResetBits(GPIOB,GPIO_Pin_6)

#define STAT_OUT_NORMAL       GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define STAT_OUT_ERROR      GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define MAX_SPEED   60000
#define FAN_SPEED_MAX 20000
#define FAN_SPEED_MIN 4000



void wdg_init(void);



u8 rtc_flag;

uint8_t warkup_flag;


uint32_t tmcdt;


__IO uint16_t  ADCConvertedValue[15];
uint32_t ADCValue[15],ADCvolt[15];


uint16_t ADC_count;

uint32_t testzhn=0;
int VM,PWBUS,VB;

unsigned long zhuansu;
unsigned char zhuansu1;
unsigned int POWER=0;
unsigned int TEMSTATUS=0;
unsigned int test=0;
unsigned int RS=0;
unsigned int RSTATUS=0;

extern unsigned int FAN_SPEED_S,FAN_SPEED_M;
unsigned int zhn=300;
float tem;
float tem2;
float pwm;
int k;


int main()
{
	 
	int a;

	uint32_t i,t,ADC_flag;
	uint32_t j,temp;
	
	zhuansu=0;
	zhuansu1=0;
	
//	float p;
//	uint8_t a[8];
	initBase();	//基本功能初始化
	
	


	Valva_Init();
	Valva1_pwm(200);
	
	TIM_Configuration1();  
	__set_PRIMASK(0);	 

//	rtc_init();

	InitUsart2();

	TMC4671_DIS();
	
	adc1_mode_config();
	
	
	
	timeout=100;
	
	while(timeout)
	{
		clrwdt();
	}
//	adcinit();
	

	
	init_TMC4671();
	ADC_count=0;

	
	tmc4671_writeInt(0,1,0);
	
	tmcdt=tmc4671_readInt(1,0,0);
	
	if(tmcdt==0x34363731)
	{
		tmc4671_init_set();
		timeout=100;
		while(timeout)
		{
			clrwdt();
		}
		TMC4671_EN();
	}
	else
	{	
		TMC4671_DIS();
			
	}

	timeout=100;
	while(timeout)
	{
		clrwdt();
	}



	for(i=0;i<3;i++)
		{
			ADCValue[i]=0;	
		}
		
	
		   
		
		
	while(1)
	{
		
		
		
		if(FAN_SPEED_MIN>FAN_SPEED_M || FAN_SPEED_M>FAN_SPEED_MAX)
			{
				LED_ERROR_ON;
			}
		else
			{
				LED_ERROR_OFF;
			}
			
		
		clrwdt();
		
//		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3) == 0 ) 
//		{
//			key_count++;
//			if(key_count==10)
//			{
//				zhuansu++;
//				if(zhuansu>6)
//					zhuansu=0;
//				else;
//			}
//			else;
//		}
//		else
//		{
//			key_count=0;
//		}
		
		
		if(ADC_count<4)
		{
			ADC_count++;
			for(j=0;j<6;j++)
			{
				ADCValue[j]=ADCValue[j]+ADCConvertedValue[j];
			}
			t=10;
			while(t)
			{
				t--;
				clrwdt();
			}	
		}
		if(ADC_count>=4)
		{
			ADC_count=0;
			for(j=0;j<6;j++)
			{
				ADCvolt[j]=ADCValue[j]/4;
				ADCvolt[j]=ADCvolt[j]*3300;
				ADCvolt[j]=ADCvolt[j]>>12;
				ADCValue[j]=0;
			}
		}
		
		VM=(float)ADCvolt[3]*6.77;
		PWBUS=(float)ADCvolt[4]*6.77;
        VB=(float)ADCvolt[0]*6.77;
		tem=temp_data(ADCvolt[2])*0.01+tem*0.99;
		tem2=temp_data2(ADCvolt[2])*0.01+tem2*0.99;
        
        pwm=3*tem-130;
        if(tem<-40 || tem>72)
        {    
            Valva1_pwm(100);
            TEMSTATUS=0;
        }
        else if(tem>50)
        {
            Valva1_pwm(pwm);
            TEMSTATUS=1;
        }
        else
        {           
            Valva1_pwm(20);
            TEMSTATUS=1;
        }
        
//        k=20;
//        pwm=(100-k)*tem/30+100-60*(100-k)/30;
//        if(tem<-40 || tem>65)
//        {    
//            Valva1_pwm(100);
//            TEMSTATUS=0;
//        }
//        else if(tem>30)
//        {
//            Valva1_pwm(pwm);
//            TEMSTATUS=1;
//        }
//        else
//        {           
//            Valva1_pwm(k);
//            TEMSTATUS=1;
//        }
//		if(tem<-40)
//			Valva1_pwm(100);
//		else if(tem<30)
//			Valva1_pwm(30);
//		else if(tem<40)
//			Valva1_pwm(50);
//		else if(tem<50)
//			Valva1_pwm(70);
//		else
//			Valva1_pwm(100);
		
//		zhuansu++;
//		if(zhuansu>40000)
//			zhuansu=0;
//		else;
//		
//		if(zhuansu>20000)
//			zhuansu1=0;
//		else
//			zhuansu1=1;
		if(PWBUS<3500 && PWBUS>1800)
		{
			POWER=1;
		}
		else
		{
			POWER=0;
		}
	//	if((VM-PWBUS)>500)
		if(VM>3000)
		{
			LED_OV_ON;
			R_ON;
            RS=1;
		}
		else
		{
			LED_OV_OFF;
			R_OFF;
            RS=0;
		}

        if(RS==0)
		{
			if(VB<100)
                RSTATUS=0;
            else
                RSTATUS=1;
		}
		else
		{
			RSTATUS=1;
		}
		
//		if(VM>3000)
//			LED_ERROR_ON;
//		else
//			LED_ERROR_OFF;
        if(POWER==1 && TEMSTATUS==1 && RSTATUS==1)
            STAT_OUT_NORMAL;
        else
            STAT_OUT_ERROR;		
		
//		if(zhuansu1==0)
//			testzhn=300;
//		else
//			testzhn=2000;
//		ADCvolt[1]=testzhn;				
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8) == 1 ) 
			ADCvolt[1]=0;
		else;  
		
//		TMC4671_EN();
		
//		ADCvolt[1]=test;
		
//		if(ADCvolt[1]>=2825)
//		{
//			ADCvolt[1]=2825;
//		}
		
//		if(ADCvolt[1]>=250 && POWER==1 && TEMSTATUS==1 && RSTATUS==1)
        if(test>=1 && POWER==1 && TEMSTATUS==1 && RSTATUS==1)
		{
			TMC4671_EN();
//			temp=ADCvolt[1];
            temp=test;
			temp=temp*MAX_SPEED;
			temp=temp/3000;
			speed_v=temp;
			speed_v=0-speed_v;
			tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000002);
		// Rotate right
			tmc4671_writeInt(0, TMC4671_PID_VELOCITY_TARGET, speed_v);
			ADC_flag=1;
			
		}
		else
		{
				if(ADC_flag)
				{
					ADC_flag=0;
					tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000002);
				// Rotate right
					tmc4671_writeInt(0, TMC4671_PID_VELOCITY_TARGET, 0);
					TMC4671_DIS();
				}
		}




	}
	while(1)
	{



	}
}




//系统基本功能初始化入口
void initBase(void)
{
    SetSysClockTo16();
	RCC_Configuration();   	// System Clocks Configuration 
    NVIC_Configuration();	// 中断 NVIC configuration 
    GPIO_Configuration();	// Configure the GPIO ports 

}


void GPIO_Configuration(void)
{ 
    GPIO_InitTypeDef GPIO_InitStructure;      
		

		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//LED
//    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//LED
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//LED
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//LED
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//xiefangdianzu
//    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//		GPIO_ResetBits(GPIOA,GPIO_Pin_10);
        
        		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//xiefangdianzu
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
        
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//STATOUT
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
				GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//KEY
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//OVER_I
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//LED
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//LED
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


		
		
		
	////////////////////////////////////////////////////////////////////////

//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_Init(GPIOC, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOC,GPIO_Pin_15);
	
	////////////////////////////////////////////////////////////////////////
	


		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin =  ENIN_TMC4671_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_ENIN_TMC4671, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin =  ENOUT_TMC4671_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_ENOUT_TMC4671, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin =  STATUS_TMC4671_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIO_STATUS_TMC4671, &GPIO_InitStructure);
		
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_InitStructure.GPIO_Pin =  IO0_TMC4671_Pin;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_Init(GPIO_IO0_TMC4671, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_InitStructure.GPIO_Pin =  IO1_TMC4671_Pin;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//    GPIO_Init(GPIO_IO1_TMC4671, &GPIO_InitStructure);
		

		


}   

/*********************************************************
*函数功能: 中断设置
*输    入：无
*输    出：无
*作    者: 
*建立日期: 2011.07.05
**********************************************************/
void NVIC_Configuration(void)
{
//    NVIC_InitTypeDef NVIC_InitStructure;
  
    #ifdef   VECT_TAB_RAM
    // Set the Vector Table base location at 0x20000000 
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  
    #else  
    // Set the Vector Table base location at 0x08000000 
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    #endif

    /* Configure the NVIC Preemption Priority Bits */  


}
/*********************************************
*函数功能: 始能端口
*输    入：无
*输    出：无
*作    者: 
*建立日期: 2011.07.05
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



void TIM_Configuration1(void)
{
  
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//打开TIM2的时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//打开TIM2的时钟
	
	TIM_DeInit(TIM2);
    TIM_InternalClockConfig(TIM2);
//    TIM_TimeBaseStructure.TIM_Period = 99;
    TIM_TimeBaseStructure.TIM_Period = 9999;
    TIM_TimeBaseStructure.TIM_Prescaler =55;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    // TIM_PrescalerConfig(TIM2, 36, TIM_PSCReloadMode_Immediate);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM2, ENABLE);
    //TIM_ARRPreloadConfig(TIM5, ENABLE);      
          
    TIM_DeInit(TIM3);
    TIM_InternalClockConfig(TIM3);
//    TIM_TimeBaseStructure.TIM_Period = 99;
    TIM_TimeBaseStructure.TIM_Period = 999;
    TIM_TimeBaseStructure.TIM_Prescaler =55;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    // TIM_PrescalerConfig(TIM2, 36, TIM_PSCReloadMode_Immediate);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);
    /* TIM IT enable
       Enable TIM2 Update interrupt [TIM2溢出中断允许]*/
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);

	    /*TIM2中断使能*/
//    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn ;    //中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;   //优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

	        

    
    /*TIM3中断使能*/
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn ;    //中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;   //优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void SetSysClockTo16(void)
{
  ErrorStatus HSEStartUpStatus;
  /* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/   
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
    {}

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
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE);
	WWDG_SetPrescaler(WWDG_Prescaler_8);
	WWDG_SetWindowValue(0x7f);
	WWDG_SetCounter(0x7f);
	WWDG_Enable(0x7f);
}


void adc1_mode_config(void)
{
	unsigned char NUM_CH=6;
	
	
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);		//Enable DMA clock 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	//Enable ADC1 and GPIOC clock
	
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 //ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADCConvertedValue;//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = NUM_CH;//5;//ZHN20211123
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址不固定
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
 	/* Enable DMA channel1 */
//	DMA_Cmd(DMA1_Channel1, ENABLE);   //以前的位置,出现错位

	/*配置ADC时钟，为PCLK2的4分频，*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div4); 

	/* ADC1 configuration */
	ADC_DeInit(ADC1);		//复位ADC1
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 //开启扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = NUM_CH;//5;	 	//要转换的通道数目5//ZHN20211123
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//配置ADC1 为239，11个采样周期，序列为1  	
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 6, ADC_SampleTime_71Cycles5);

 
//Enable the temperature sensor and vref internal channel
	ADC_TempSensorVrefintCmd(ENABLE);	
/*	ADC_InjectedSequencerLengthConfig(ADC1, 1); //注入方式
	ADC_InjectedChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_71Cycles5);	// ADC1 injected channel Configuration  
	ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_None);		// ADC1 injected external trigger configuration 
	ADC_InjectedDiscModeCmd(ADC1, DISABLE);
	
	ADC_ExternalTrigInjectedConvCmd(ADC1, ENABLE); */
/////////////////////////////////////////////////////////////	

	ADC_DMACmd(ADC1, ENABLE);	// Enable ADC1 DMA 		
	ADC_Cmd(ADC1, ENABLE);	// Enable ADC1 	 
	ADC_ResetCalibration(ADC1);	// 复位校准寄存器  
	while(ADC_GetResetCalibrationStatus(ADC1));	// 等待校准寄存器复位完成 	
	ADC_StartCalibration(ADC1);	 // ADC校准 
	while(ADC_GetCalibrationStatus(ADC1));	 // 等待校准完成
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);	  // 由于没有采用外部触发，所以使用软件触发ADC转换 
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);   //在ADC_SoftwareStartConvCmd(ADC1, ENABLE);不然容易出现ADC采集出现错位
}

void adcinit(void)
{
	ADC_InitTypeDef ADC_InitStructure;      //ADC????????
	DMA_InitTypeDef DMA_InitStructure;      //DMA????????	
	/* Enable DMA2 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
 
  /* DMA1 channel1 configuration ----------------------------------------------*/
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_BASE+0x4c;    //DMA????????
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADCConvertedValue;   //???????
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//DMA??????SRC??,????????
  DMA_InitStructure.DMA_BufferSize = 5;		   //DMA????,1?,???DMA_MemoryDataSize
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//???????,????????
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//?????????,????????
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //?????????16?
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  //DMA?????,HalfWord???16?
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   //????,???????
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	//DMA????
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		  //M2M????
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);          
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  
  /* ADC1 configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  //???????
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;		  //??????
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;   //????????
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ADC????,????
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;   //????,ADC?12??,?????
  ADC_InitStructure.ADC_NbrOfChannel = 5;	 //?????,1?
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1 regular channel10 configuration ADC???, ?10??? ????1,???? */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_71Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_71Cycles5);
  ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 3, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 4, ADC_SampleTime_71Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 5, ADC_SampleTime_71Cycles5);
  
  /* Enable ADC1 DMA */
  ADC_DMACmd(ADC1, ENABLE);	  //ADC??,??
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);  //??ADC1
  
  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);	  //????
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));  //????????
  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);		//????
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));	   //??????
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);	//??????,ADC??DMA???????RAM??
}


 void Valva_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
	TIM_TimeBaseInitTypeDef	TimeBaseInitStruct;
//	DMA_InitTypeDef DMA_InitStructure;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//′ò?aTIM2μ?ê±?ó

	TIM_Cmd(TIM4, DISABLE);
	TIM_DeInit(TIM4);
  TIM_InternalClockConfig(TIM4);
  
  
  /* ---------------------------------------------------------------
  TIM2 Configuration: Output Compare Toggle Mode:
  --------------------------------------------------------------- */
   /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period =1119;
  TIM_TimeBaseStructure.TIM_Prescaler = 1;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  /* Output Compare Toggle Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;  
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC4Init(TIM4, &TIM_OCInitStructure);





  TIM_Cmd(TIM4, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn ;    //中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;   //优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);		//开启TIM1的外设时钟
 
 	TIM_DeInit(TIM1); 
	TimeBaseInitStruct.TIM_Period = 0xFFFF;
	TimeBaseInitStruct.TIM_Prescaler = 0x00;
	TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TimeBaseInitStruct); 
	
	TIM_TIxExternalClockConfig(TIM1, TIM_TIxExternalCLK1Source_TI1, TIM_ICPolarity_Rising,0);
	TIM_ETRClockMode2Config(TIM1,TIM_ExtTRGPSC_OFF,TIM_ExtTRGPolarity_NonInverted,0);
	TIM_SetCounter(TIM1, 0);
	TIM_Cmd(TIM1, ENABLE);
	
 NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;   //中断占先等级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	

    
}

void Valva1_pwm(uint16_t dt)
{
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
//	/* Output Compare Toggle Mode configuration: Channel4 */
//  TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM1;  
//  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//  TIM_OCInitStructure.TIM_Pulse = dt;
//  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
if(dt>100)
	dt=100;
if(dt<0)
	dt=0;
TIM_SetCompare4(TIM4,(1119*dt)/100);
}


void get_sudu(void)
{
		uint32_t temp;
		if(RI2_flag)
		{
				temp=((uint32_t)Rx_Buf2[3]-0x30)*10000;	
				temp=temp+((uint32_t)Rx_Buf2[4]-0x30)*1000;
				temp=temp+((uint32_t)Rx_Buf2[5]-0x30)*100;
				temp=temp+((uint32_t)Rx_Buf2[6]-0x30)*10;
				temp=temp+((uint32_t)Rx_Buf2[7]-0x30);
				RI2_flag=0;
//				speed_v=(uint16_t)temp;
//			speed_v=0-speed_v;
				//usart2_SendString("OK");
			Rx_Buf2[9]='\0';
			//usart1_SendString(Rx_Buf1);
			usart2_send('S');
			usart2_send('E');
			usart2_send('=');
			usart2_send(Rx_Buf2[3]);
			usart2_send(Rx_Buf2[4]);
			usart2_send(Rx_Buf2[5]);
			usart2_send(Rx_Buf2[6]);
			usart2_send(Rx_Buf2[7]);
			usart2_send(',');
			
//			tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000002);

//			tmc4671_writeInt(0, TMC4671_PID_VELOCITY_TARGET, speed_v);
		}
}

//void TIM4_PWM_Init(u16 arr,u16 psc)
//{  
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;
// 
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);// 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);  //
//	                                                                     	

// 
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
// 
// 
//	
//	TIM_TimeBaseStructure.TIM_Period = arr; //
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //
//	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //
//	
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //
//	TIM_OCInitStructure.TIM_Pulse = 0; //
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //
//	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //
// 
//  TIM_CtrlPWMOutputs(TIM4,ENABLE);	//
// 
//	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //
//	
//	TIM_ARRPreloadConfig(TIM4, ENABLE); //
//	
//	TIM_Cmd(TIM4, ENABLE);  //
// 
//}

float temp_data(uint32_t adc_V)

{

	long Rt=0;
	
	long VCC=0;

	float Rp=10000;

	float T2=273.15+25;

//	float Bx=3020;
	float Bx=3490;
	
	float Ka=273.15;

	float vol=0;

	float temp=0;	

	VCC=(4096*1200)/ADCConvertedValue[5];

	Rt=(10000*adc_V)/(VCC-adc_V);

	temp=1/(1/T2+log(Rt/Rp)/Bx)-Ka+0.5;	
		
	return temp;
}

float temp_data2(uint32_t adc_V)

{

	long Rt=0;
	
	long VCC=0;

	float Rp=10000;

	float T2=273.15+25;

//	float Bx=3020;
	float Bx=3020;
	
	float Ka=273.15;

	float vol=0;

	float temp=0;	

	VCC=(4096*1200)/ADCConvertedValue[5];

	Rt=(10000*adc_V)/(VCC-adc_V);

	temp=1/(1/T2+log(Rt/Rp)/Bx)-Ka+0.5;	
		
	return temp;
}
