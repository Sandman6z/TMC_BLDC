#include "IIC_FMEEP.h"
#include  "Global.h"

void GPIO_FMEEP_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
  /* Configure I2C1 pins: SCL and SDA */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);


}

/*******************************************************************************
* Function Name  : I2C_Configuration
* Description    : I2C Configuration
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void I2C1_Configuration(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = EEPROM_ADDRESS_READ;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = 100000;
  
  /* I2C Peripheral Enable */
  I2C_Cmd(I2C1, ENABLE);
  /* Apply I2C configuration after enabling it */
  I2C_Init(I2C1, &I2C_InitStructure);

	/*允许1字节1应答模式*/
	I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    
}

void eeprom_init(void)
{
	I2C1_Configuration();
	GPIO_FMEEP_init();
		
}
/*******************************************************************************
* Function Name  : I2C_EE_ByteWrite
* Description    : Writes one byte to the I2C EEPROM.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the EEPROM.
*                  - WriteAddr : EEPROM's internal address to write to.
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_EE_ByteWrite(u8 pBuffer, u16 WriteAddr)
{
  eeprom_init();
  I2C_AcknowledgeConfig(I2C1, ENABLE);//使能应答
  /* Send STRAT condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  ms_timeout=5;
  /* Test on EV5 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))&ms_timeout){clrwdt();}  
  if(!ms_timeout)
  {
	  I2C_GenerateSTART(I2C1, ENABLE);
	  ms_timeout=5;
	  /* Test on EV5 and clear it */
	  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))&ms_timeout){clrwdt();}
  }
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS_WRITE, I2C_Direction_Transmitter);
  ms_timeout=5;
  /* Test on EV6 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&ms_timeout){clrwdt();}
      
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, (WriteAddr>>8)&0xff);
  ms_timeout=5;
  /* Test on EV8 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&ms_timeout){clrwdt();}
   
  I2C_SendData(I2C1, (WriteAddr)&0xff);
  ms_timeout=5;
  /* Test on EV8 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&ms_timeout){clrwdt();}
  /* Send the byte to be written */
  I2C_SendData(I2C1, pBuffer); 
  ms_timeout=5; 
  /* Test on EV8 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&ms_timeout){clrwdt();}
  
  /* Send STOP condition */
  I2C_GenerateSTOP(I2C1, ENABLE);
  ms_timeout=2;
  while(ms_timeout){clrwdt();}
}

/*******************************************************************************
* Function Name  : I2C_EE_BufferRead
* Description    : Reads a block of data from the EEPROM.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the EEPROM.
*                  - ReadAddr : EEPROM's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the EEPROM.
* Output         : None
* Return         : None
*******************************************************************************/
u8 I2C_EE_ByteRead(u16 ReadAddr)
{  
  u8 pBuffer;
  eeprom_init();
  I2C_AcknowledgeConfig(I2C1, ENABLE);//使能应答
  //*((u8 *)0x4001080c) |=0x80; 
//    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY)); // Added by Najoua 27/08/2008
    
    
  /* Send START condition */
  I2C_GenerateSTART(I2C1, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  ms_timeout=5;
  /* Test on EV5 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))& ms_timeout){clrwdt();}
  if(!ms_timeout)
  {
	  I2C_GenerateSTART(I2C1, ENABLE);
	  ms_timeout=5;
	  /* Test on EV5 and clear it */
	  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))&ms_timeout){clrwdt();}
  }
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS_WRITE, I2C_Direction_Transmitter);

  ms_timeout=5;
  /* Test on EV6 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))&ms_timeout){clrwdt();}
  
  /* Clear EV6 by setting again the PE bit */
//  I2C_Cmd(I2C1, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, (ReadAddr>>8)&0xff);  

  ms_timeout=5;
  /* Test on EV8 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&ms_timeout){clrwdt();}
   /* Send the EEPROM's internal address to write to */
  I2C_SendData(I2C1, ReadAddr&0xff);  

  ms_timeout=5;
  /* Test on EV8 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))&ms_timeout){clrwdt();}
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(I2C1, ENABLE);
  
  ms_timeout=5;
  /* Test on EV5 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))&ms_timeout){clrwdt();}
  if(!ms_timeout)
  {
	  I2C_GenerateSTART(I2C1, ENABLE);
	  ms_timeout=5;
	  /* Test on EV5 and clear it */
	  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))&ms_timeout){clrwdt();}
  }
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(I2C1, EEPROM_ADDRESS_READ, I2C_Direction_Receiver);
  
  ms_timeout=5;
  /* Test on EV6 and clear it */
  while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))&ms_timeout){clrwdt();}
  
  /* While there is data to be read */


      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(I2C1, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(I2C1, ENABLE);

	ms_timeout=5;
    /* Test on EV7 and clear it */
    while((!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))&ms_timeout){clrwdt();}  
    {      
      /* Read a byte from the EEPROM */
      pBuffer = I2C_ReceiveData(I2C1);
    }   
	 ms_timeout=2;
 	 while(ms_timeout){clrwdt();}

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(I2C1, ENABLE);
  return pBuffer;
}

