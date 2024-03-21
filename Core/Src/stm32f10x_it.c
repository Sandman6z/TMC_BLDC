/**
 ******************************************************************************
 * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
 * @author  MCD Application Team
 * @version V3.5.0
 * @date    08-April-2011
 * @brief   Main Interrupt Service Routines.
 *          This file provides template for all exceptions handler and
 *          peripherals interrupt service routine.
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
#include "stm32f10x_it.h"
#include "../Inc/main.h"
#include "../../User/bsp_uart_process.h"

#define TIMEcycle 50       // 1 seconds per cycle
#define TIME_MS_CYCLE 2    // ÿ1ms
#define TIME_SEC_CYCLE 100 // 1 seconds per cycle

unsigned int timeout;
unsigned char timeout_18B20;
unsigned char TIMEcount;
unsigned char ms_timeout;
unsigned char LED_stat;
unsigned char nRF_Sec;
unsigned int FAN_SPEED_S, FAN_SPEED_M;
unsigned int FAN_COUNT;
u8 DMA_flag = 0;
u8 motor_flag, Beep_count;
u8 nRec1, R_Data1, Rx_Buf1[15], RI1_flag;
u8 nRec2, R_Data2, Rx_Buf2[15], RI2_flag;
u8 Tx_Buf2[30];
u16 Key_Read, Key_Save, Key_Count, Key_Value, Key_Value_Save, Key_Press_flag;

/** @addtogroup STM32F10x_StdPeriph_Template
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/**
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Memory Manage exception.
 * @param  None
 * @retval None
 */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Bus Fault exception.
 * @param  None
 * @retval None
 */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles Usage Fault exception.
 * @param  None
 * @retval None
 */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}

/**
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

/**
 * @brief  This function handles PendSVC exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}

/**
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
}

/*
********************************************************************
* Function Name  : TIM2_IRQHandler
* Description    : ��ʱ��2�жϷ������
* Input          : None
* Output         : None
* Return         : None
********************************************************************
*/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        if (timeout)
            timeout--;

        TIMEcount++;

        if (TIMEcount >= TIME_SEC_CYCLE)
        {
            TIMEcount = 0;

            TIM_Cmd(TIM1, DISABLE);
            TIM_SetCounter(TIM1, 0);
            TIM_Cmd(TIM1, ENABLE);
           
        }
    }
}

/*
********************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : ��ʱ��3�жϷ������
* Input          : None
* Output         : None
* Return         : None
********************************************************************
*/
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        if (ms_timeout != 0)
            ms_timeout--;
    }
}

void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

        // 每两秒切换gTargetValue的值
        if (gTargetValue == 6000)
        {
            gTargetValue = 60000;
        }
        else
        {
            gTargetValue = 6000;
        }
    }
}
/***********************************************************
 *��������: SPI2�жϺ�������PCM1770������Ƶ����
 *��    �룺��
 *��    ������
 *��    ��:
 *��������: 2011.10.11
 ************************************************************/
void SPI2_IRQHandler(void)
{
    if ((SPI_I2S_GetITStatus(SPI2, SPI_I2S_IT_TXE) == SET))
    {
        /* Send data on the SPI2 and Check the current commands */
        //        I2S_CODEC_DataTransfer();
        SPI_I2S_ClearFlag(SPI2, 0xffff);
    }
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles RTC global interrupt request.
 * @param  None
 * @retval None
 */
void RTCAlarm_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
    {
        /* Clear EXTI line17 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line17);

        /* Check if the Wake-Up flag is set */
        if (PWR_GetFlagStatus(PWR_FLAG_WU) != RESET)
        {
          /* Clear Wake Up flag */
          PWR_ClearFlag(PWR_FLAG_WU);
        }
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
        /* Clear RTC Alarm interrupt pending bit */
        RTC_ClearITPendingBit(RTC_IT_ALR);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
        rtc_flag = 1;
    }
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

void OTG_FS_IRQHandler(void)
{
}
/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        EXTI_ClearITPendingBit(EXTI_Line0);
        
    }
}
/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
void EXTI3_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line3) != RESET)
        EXTI_ClearITPendingBit(EXTI_Line3);
}
/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) != RESET)
        EXTI_ClearITPendingBit(EXTI_Line5);

}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles DMA1 Channel 6 interrupt request.
 * @param  None
 * @retval None
 */
void DMA1_Channel5_IRQHandler(void)
{
    /* Test on DMA1 Channel6 Transfer Complete interrupt */
    if (DMA_GetITStatus(DMA1_IT_TC5))
    {
        /* Get Current Data Counter value after complete transfer */
        //    CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel5);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        DMA_ClearITPendingBit(DMA1_IT_GL5);
        DMA_ClearITPendingBit(DMA1_IT_TC5);
    }
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles DMA1 Channel 6 interrupt request.
 * @param  None
 * @retval None
 */
void DMA2_Channel5_IRQHandler(void)
{
    /* Test on DMA1 Channel6 Transfer Complete interrupt */
    if (DMA_GetITStatus(DMA2_IT_TC5))
    {
        /* Get Current Data Counter value after complete transfer */
        //    CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel5);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        TIM_Cmd(TIM5, DISABLE);
        DMA_ClearITPendingBit(DMA2_IT_GL5);
        DMA_ClearITPendingBit(DMA2_IT_TC5);
    }
}

void DMA2_Channel4_IRQHandler(void)
{
    /* Test on DMA1 Channel6 Transfer Complete interrupt */
    if (DMA_GetITStatus(DMA2_IT_TC5))
    {
        /* Get Current Data Counter value after complete transfer */
        //    CurrDataCounterEnd = DMA_GetCurrDataCounter(DMA1_Channel5);
        /* Clear DMA1 Channel6 Half Transfer, Transfer Complete and Global interrupt pending bits */
        TIM_Cmd(TIM5, DISABLE);
        DMA_ClearITPendingBit(DMA2_IT_GL5);
        DMA_ClearITPendingBit(DMA2_IT_TC5);
    }
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles USARTy global interrupt request.
 * @param  None
 * @retval None
 */
void UART5_IRQHandler(void)
{
    if (USART_GetITStatus(UART5, USART_IT_TXE) != RESET)
        USART_SendData(UART5, 0);
    if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
        USART_ReceiveData(UART5); // ��������
}
/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles USARTy global interrupt request.
 * @param  None
 * @retval None
 */
void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
        USART_SendData(USART1, 0);
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
        USART_ReceiveData(USART1);                          // ��������
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles USARTy global interrupt request.
 * @param  None
 * @retval None
 */
void UART4_IRQHandler(void)
{
    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
        USART_ReceiveData(UART4);                           // ��������
    if (USART_GetITStatus(UART4, USART_IT_TXE) != RESET)
        USART_SendData(UART4, 0);
}

/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
 * @brief  This function handles USARTy global interrupt request.
 * @param  None
 * @retval None
 */
void USART2_IRQHandler(void)
{

}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
 * @brief  This function handles PPP interrupt request.
 * @param  None
 * @retval None
 */
/*void PPP_IRQHandler(void)
{
}*/

/**
 * @}
 */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
