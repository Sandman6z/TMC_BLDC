#ifndef _UART_PROCESS_H
#define _UART_PROCESS_H
#endif

#include "stm32f10x.h"





#define GET_DATA_CMD 0x00
#define PRESSURE_CHECK_CMD 0x01
#define FLOW_CHECK_CMD 0x02


void usart2_send(u8 dt);
void usart2_SendString (unsigned char  *pt);
void usart1_send(u8 dt);
void usart1_SendString (unsigned char  *pt);
void uart4_send(u8 dt);
void uart4_SendString (unsigned char  *pt);
void uart5_send(u8 dt);
void uart5_SendString (unsigned char  *pt);





