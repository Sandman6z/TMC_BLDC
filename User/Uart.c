#include "Global.h"
#include "uart.h"


/* USART1  configuration
*函数功能: 初始化串口2
*输入参数: 无
*返回参数:无
*作    者: 
*时    间:
*说    明:
*          
******************************************************************************/

void InitUsart2(void)
{   
    USART_InitTypeDef USART_InitStructure;                          //初始化参数设置
    GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;                 //复用推挽输出
    GPIO_InitStructure.GPIO_Pin     =  GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART2 Rx (PA.3)as input floating */
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;//浮空输入 
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	
	
	USART_InitStructure.USART_BaudRate              = 9600;                    //38400
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //字长8位
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1位停止字节
    USART_InitStructure.USART_Parity                = USART_Parity_No;             //无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //无流控制
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;                   //打开Rx接收和Tx发送功能
     
    USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);  //接收缓冲区未空（使能接收中断）/*这两句没有程序也可以正常运行*/                                       
    USART_ITConfig(USART2,USART_IT_TXE, DISABLE);  //发送缓冲区空（不能发送中断)
	USART_ITConfig(USART2,USART_IT_TC, DISABLE);	//发送缓冲区空（不使能发送中断)    

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);// 使能USART外设

    NVIC_InitStructure.NVIC_IRQChannel                      =  USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
*函数功能: 初始化串口1
*输入参数: 无
*返回参数:无
*作    者: 
*时    间: 
*说    明:
*          
******************************************************************************/
void initUsart1(void)
{    
    USART_InitTypeDef USART_InitStructure;                      //初始化参数设置
    GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);


    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;          //复用推挽输出
    GPIO_InitStructure.GPIO_Pin     =  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //浮空输入 
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	USART_InitStructure.USART_BaudRate              = 115200;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //字长8位
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1位停止字节
    USART_InitStructure.USART_Parity                = USART_Parity_No;             //无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //无流控制
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;    //打开Rx接收和Tx发送功能
     
    USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);  	        //（使能接收中断）									
    USART_ITConfig(USART1,USART_IT_TXE, DISABLE);           //发送缓冲区空（不使能发送中断)
	USART_ITConfig(USART1,USART_IT_TC, DISABLE);	        //发送缓冲区空（不使能发送中断)

    USART_Init(USART1, &USART_InitStructure);               //根据指定的参数初始化外设USART1寄存器    
    USART_Cmd(USART1, ENABLE);                              // 使能USART外设

    NVIC_InitStructure.NVIC_IRQChannel                      =  USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/******************************************************************************
*函数功能: 初始化串口3
*输入参数: 无
*返回参数:无
*作    者: 
*时    间: 
*说    明:
*          
******************************************************************************/
void initUsart3(void)
{   
    USART_InitTypeDef USART_InitStructure;                          //初始化参数设置
                         
    USART_InitStructure.USART_BaudRate              = 9600;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //字长8位
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1位停止字节
    USART_InitStructure.USART_Parity                = USART_Parity_No;             //无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //无流控制
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;                   //打开Rx接收和Tx发送功能
     
    USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);  //（使能接收中断）//										
    USART_ITConfig(USART3,USART_IT_TXE, DISABLE);	//发送缓冲区空（不使能发送中断)
	USART_ITConfig(USART3,USART_IT_TC, DISABLE);	//发送缓冲区空（不使能发送中断)

    USART_Init(USART3, &USART_InitStructure);  //根据指定的参数初始化外设USART1寄存器
    USART_Cmd(USART3, ENABLE);// 使能USART外设
}

/******************************************************************************
*函数功能: 初始化串口5
*输入参数: 无
*返回参数:无
*作    者: 
*时    间: 
*说    明:
*          
******************************************************************************/
void initUsart5(void)
{   
    USART_InitTypeDef USART_InitStructure;                          //初始化参数设置
    USART_InitStructure.USART_BaudRate              = 9600;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //字长8位
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1位停止字节
    USART_InitStructure.USART_Parity                = USART_Parity_Even;             //偶校验
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //无流控制
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;                   //打开Rx接收和Tx发送功能
     
    USART_ITConfig(UART5,USART_IT_RXNE, ENABLE);  //（使能接收中断）//										
    USART_ITConfig(UART5,USART_IT_TXE, DISABLE);	//发送缓冲区空（不使能发送中断)
	USART_ITConfig(UART5,USART_IT_TC, DISABLE);	//发送缓冲区空（不使能发送中断)

    USART_Init(UART5, &USART_InitStructure);  //根据指定的参数初始化外设USART5寄存器
    USART_Cmd(UART5, ENABLE);// 使能USART外设
}


/******************************************************************************
*函数功能: 初始化串口4
*输入参数: 无
*返回参数:无
*作    者: 
*时    间: 
*说    明: 
*          
******************************************************************************/
void initUsart4(void)
{ 
    USART_InitTypeDef USART_InitStructure;                          //初始化参数设置
    USART_InitStructure.USART_BaudRate              = 9600;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //字长8位
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1位停止字节
    USART_InitStructure.USART_Parity                = USART_Parity_Even;             //偶校验
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //无流控制
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;                   //打开Rx接收和Tx发送功能
     
    USART_ITConfig(UART4,USART_IT_RXNE, ENABLE);  //（使能接收中断）//										
    USART_ITConfig(UART4,USART_IT_TXE, DISABLE);	//发送缓冲区空（不使能发送中断)
	USART_ITConfig(UART4,USART_IT_TC, DISABLE);	//发送缓冲区空（不使能发送中断)

    USART_Init(UART4, &USART_InitStructure);  //根据指定的参数初始化外设USART5寄存器
    USART_Cmd(UART4, ENABLE);// 使能USART外设

	nRec1=0;
	RI1_flag=0;
}
