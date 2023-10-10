/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IIC_FMEEP_H
#define __IIC_FMEEP_H 
#endif



#include "stm32f10x.h"

#define EEPROM_ADDRESS_WRITE 0xa0
#define EEPROM_ADDRESS_READ 0xa0    //0xa1

void GPIO_FMEEP_init(void);
void I2C1_Configuration(void);
void I2C_EE_ByteWrite(u8 pBuffer, u16 WriteAddr);
u8 I2C_EE_ByteRead(u16 ReadAddr);
void eeprom_init(void);

