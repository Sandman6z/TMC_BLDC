#include "Global.h"
#include "uart.h"


/* USART1  configuration
*��������: ��ʼ������2
*�������: ��
*���ز���:��
*��    ��: 
*ʱ    ��:
*˵    ��:
*          
******************************************************************************/

void InitUsart2(void)
{   
    USART_InitTypeDef USART_InitStructure;                          //��ʼ����������
    GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 , ENABLE);
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;                 //�����������
    GPIO_InitStructure.GPIO_Pin     =  GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_10MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure USART2 Rx (PA.3)as input floating */
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;//�������� 
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	
	
	USART_InitStructure.USART_BaudRate              = 9600;                    //38400
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //�ֳ�8λ
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1λֹͣ�ֽ�
    USART_InitStructure.USART_Parity                = USART_Parity_No;             //����żУ��
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //��������
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;                   //��Rx���պ�Tx���͹���
     
    USART_ITConfig(USART2,USART_IT_RXNE, ENABLE);  //���ջ�����δ�գ�ʹ�ܽ����жϣ�/*������û�г���Ҳ������������*/                                       
    USART_ITConfig(USART2,USART_IT_TXE, DISABLE);  //���ͻ������գ����ܷ����ж�)
	USART_ITConfig(USART2,USART_IT_TC, DISABLE);	//���ͻ������գ���ʹ�ܷ����ж�)    

    USART_Init(USART2, &USART_InitStructure);
    USART_Cmd(USART2, ENABLE);// ʹ��USART����

    NVIC_InitStructure.NVIC_IRQChannel                      =  USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 6;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/******************************************************************************
*��������: ��ʼ������1
*�������: ��
*���ز���:��
*��    ��: 
*ʱ    ��: 
*˵    ��:
*          
******************************************************************************/
void initUsart1(void)
{    
    USART_InitTypeDef USART_InitStructure;                      //��ʼ����������
    GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);


    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;          //�����������
    GPIO_InitStructure.GPIO_Pin     =  GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //�������� 
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	USART_InitStructure.USART_BaudRate              = 115200;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //�ֳ�8λ
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1λֹͣ�ֽ�
    USART_InitStructure.USART_Parity                = USART_Parity_No;             //����żУ��
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //��������
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;    //��Rx���պ�Tx���͹���
     
    USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);  	        //��ʹ�ܽ����жϣ�									
    USART_ITConfig(USART1,USART_IT_TXE, DISABLE);           //���ͻ������գ���ʹ�ܷ����ж�)
	USART_ITConfig(USART1,USART_IT_TC, DISABLE);	        //���ͻ������գ���ʹ�ܷ����ж�)

    USART_Init(USART1, &USART_InitStructure);               //����ָ���Ĳ�����ʼ������USART1�Ĵ���    
    USART_Cmd(USART1, ENABLE);                              // ʹ��USART����

    NVIC_InitStructure.NVIC_IRQChannel                      =  USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 5;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/******************************************************************************
*��������: ��ʼ������3
*�������: ��
*���ز���:��
*��    ��: 
*ʱ    ��: 
*˵    ��:
*          
******************************************************************************/
void initUsart3(void)
{   
    USART_InitTypeDef USART_InitStructure;                          //��ʼ����������
                         
    USART_InitStructure.USART_BaudRate              = 9600;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //�ֳ�8λ
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1λֹͣ�ֽ�
    USART_InitStructure.USART_Parity                = USART_Parity_No;             //����żУ��
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //��������
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;                   //��Rx���պ�Tx���͹���
     
    USART_ITConfig(USART3,USART_IT_RXNE, ENABLE);  //��ʹ�ܽ����жϣ�//										
    USART_ITConfig(USART3,USART_IT_TXE, DISABLE);	//���ͻ������գ���ʹ�ܷ����ж�)
	USART_ITConfig(USART3,USART_IT_TC, DISABLE);	//���ͻ������գ���ʹ�ܷ����ж�)

    USART_Init(USART3, &USART_InitStructure);  //����ָ���Ĳ�����ʼ������USART1�Ĵ���
    USART_Cmd(USART3, ENABLE);// ʹ��USART����
}

/******************************************************************************
*��������: ��ʼ������5
*�������: ��
*���ز���:��
*��    ��: 
*ʱ    ��: 
*˵    ��:
*          
******************************************************************************/
void initUsart5(void)
{   
    USART_InitTypeDef USART_InitStructure;                          //��ʼ����������
    USART_InitStructure.USART_BaudRate              = 9600;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //�ֳ�8λ
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1λֹͣ�ֽ�
    USART_InitStructure.USART_Parity                = USART_Parity_Even;             //żУ��
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //��������
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;                   //��Rx���պ�Tx���͹���
     
    USART_ITConfig(UART5,USART_IT_RXNE, ENABLE);  //��ʹ�ܽ����жϣ�//										
    USART_ITConfig(UART5,USART_IT_TXE, DISABLE);	//���ͻ������գ���ʹ�ܷ����ж�)
	USART_ITConfig(UART5,USART_IT_TC, DISABLE);	//���ͻ������գ���ʹ�ܷ����ж�)

    USART_Init(UART5, &USART_InitStructure);  //����ָ���Ĳ�����ʼ������USART5�Ĵ���
    USART_Cmd(UART5, ENABLE);// ʹ��USART����
}


/******************************************************************************
*��������: ��ʼ������4
*�������: ��
*���ز���:��
*��    ��: 
*ʱ    ��: 
*˵    ��: 
*          
******************************************************************************/
void initUsart4(void)
{ 
    USART_InitTypeDef USART_InitStructure;                          //��ʼ����������
    USART_InitStructure.USART_BaudRate              = 9600;
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;     //�ֳ�8λ
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;          //1λֹͣ�ֽ�
    USART_InitStructure.USART_Parity                = USART_Parity_Even;             //żУ��
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //��������
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;                   //��Rx���պ�Tx���͹���
     
    USART_ITConfig(UART4,USART_IT_RXNE, ENABLE);  //��ʹ�ܽ����жϣ�//										
    USART_ITConfig(UART4,USART_IT_TXE, DISABLE);	//���ͻ������գ���ʹ�ܷ����ж�)
	USART_ITConfig(UART4,USART_IT_TC, DISABLE);	//���ͻ������գ���ʹ�ܷ����ж�)

    USART_Init(UART4, &USART_InitStructure);  //����ָ���Ĳ�����ʼ������USART5�Ĵ���
    USART_Cmd(UART4, ENABLE);// ʹ��USART����

	nRec1=0;
	RI1_flag=0;
}
