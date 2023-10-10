
#ifndef __TMC4671_H
#define __TMC4671_H
#endif

#include "stm32f10x.h"

void SPI_TMC4671_Init(void);
void GPIO_TMC4671_SPI_init(void);
u8 SPI_TMC4671_SendByte(u8 byte);
void  delay_TMC4671(uint16_t dt);
void init_TMC4671(void);
void write_TMC4671(uint8_t addr,uint32_t dt);
uint32_t Read_TMC4671(uint8_t addr);
void tmc4671_writeInt(uint8_t fg,uint8_t addr,uint32_t dt);
uint32_t tmc4671_readInt(uint8_t fg,uint8_t addr,uint32_t dt);
void tmc4671_init_set(void);



#define TMC4671_MODE_RAMP_MODE_MOTION 			0x63
#define TMC4671_PID_TORQUE_FLUX_TARGET 			0x64
#define TMC4671_PID_VELOCITY_TARGET 				0x66
#define TMC4671_MOTOR_TYPE_N_POLE_PAIRS  		0x1B
#define TMC4671_PWM_POLARITIES							0x17
#define TMC4671_PWM_MAXCNT									0x18
#define TMC4671_PWM_BBM_H_BBM_L							0x19
#define TMC4671_PWM_SV_CHOP									0x1A
#define TMC4671_ADC_I_SELECT								0x0A
#define TMC4671_dsADC_MCFG_B_MCFG_A					0x04
#define TMC4671_dsADC_MCLK_A								0x05
#define TMC4671_dsADC_MCLK_B								0x06
#define TMC4671_dsADC_MDEC_B_MDEC_A					0x07
#define TMC4671_ADC_I0_SCALE_OFFSET					0x09
#define TMC4671_ADC_I1_SCALE_OFFSET					0x08
#define TMC4671_HALL_MODE										0x33
#define TMC4671_HALL_PHI_E_PHI_M_OFFSET			0x37
#define TMC4671_PHI_E_SELECTION							0x52
#define TMC4671_VELOCITY_SELECTION					0x50
#define TMC4671_PID_TORQUE_FLUX_LIMITS			0x5E
#define TMC4671_PID_FLUX_P_FLUX_I						0x54
#define TMC4671_PID_TORQUE_P_TORQUE_I				0x56
#define TMC4671_POSITION_SELECTION					0x51
#define TMC4671_PIDOUT_UQ_UD_LIMITS					0x5D
#define TMC4671_PID_ACCELERATION_LIMITS			0x5F
#define TMC4671_PID_VELOCITY_LIMIT					0x60
#define TMC4671_PID_POSITION_LIMIT_LOW			0x61
#define TMC4671_PID_POSITION_LIMIT_HIGH			0x62
#define TMC4671_PID_VELOCITY_P_VELOCITY_I		0x58
#define TMC4671_PID_POSITION_P_POSITION_I		0x5A




