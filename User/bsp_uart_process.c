#include "bsp_uart_process.h"
#include "./main.h"

/***********************************************************************************
* Uart1_Send;
*
* Uart1;

************************************************************************************/
void usart1_send(u8 dt)
{
	/* Wait until end of transmit */
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE)== RESET)
    {
    }
    /* Write one byte in the USARTz Transmit Data Register */
    USART_SendData(USART1, dt);

}
void usart1_SendString (unsigned char  *pt)
{  

	while ((*pt!='\0'))
	{
		usart1_send (*pt++);
	}
}
/***********************************************************************************
* Uart2_Send;
*
* Uart1;

************************************************************************************/
void usart2_send(u8 dt)
{
	/* Wait until end of transmit */
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE)== RESET)
    {
    }
    /* Write one byte in the USARTz Transmit Data Register */
    USART_SendData(USART2, dt);

}
void usart2_SendString (unsigned char  *pt)
{  

	while ((*pt!='\0'))
	{
		usart2_send (*pt++);
	}
}
/***********************************************************************************
* Uart4_Send;
*
* Uart1;

************************************************************************************/
void uart4_send(u8 dt)
{
	/* Wait until end of transmit */
    while(USART_GetFlagStatus(UART4, USART_FLAG_TXE)== RESET)
    {
    }
    /* Write one byte in the USARTz Transmit Data Register */
    USART_SendData(UART4, dt);

}
void uart4_SendString (unsigned char  *pt)
{  

	while ((*pt!='\0'))
	{
		uart4_send (*pt++);
	}
}
/***********************************************************************************
* Uart5_Send;
*
* Uart1;

************************************************************************************/
void uart5_send(u8 dt)
{
	/* Wait until end of transmit */
    while(USART_GetFlagStatus(UART5, USART_FLAG_TXE)== RESET)
    {
    }
    /* Write one byte in the USARTz Transmit Data Register */
    USART_SendData(UART5, dt);

}
void uart5_SendString (unsigned char  *pt)
{  

	while ((*pt!='\0'))
	{
		uart5_send (*pt++);
	}
}
