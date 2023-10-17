#include  "TMC4671.h"
#include  "Global.h"

#define Set_SPI_MOSI()		GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define ReSet_SPI_MOSI()	GPIO_ResetBits(GPIOB,GPIO_Pin_15)

#define Set_SPI_SCK() 	   	GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define ReSet_SPI_SCK() 	GPIO_ResetBits(GPIOB,GPIO_Pin_13)

#define Get_SPI_MISO()  	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)

#define Set_SPI_TMC4671() 	GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define ReSet_SPI_TMC4671() GPIO_ResetBits(GPIOB,GPIO_Pin_12)

#define PID_TORQUE_I		800
#define PID_TORQUE_P		200

#define PID_FLUX_I			800
#define PID_FLUX_P			200

#define PID_VELOCITY_I		30
#define PID_VELOCITY_P		800

#define PID_POSITION_I		0
#define PID_POSITION_P		0

/**
 * @brief: SPI???
 * @Description: 
 * @date1
 */
void SPI_TMC4671_Init(void)
{
	SPI_InitTypeDef  SPI_InitStructure;

	SPI_InitStructure.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode 				= SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 				= SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA 				= SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS 				= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler	= SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit 			= SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial 	= 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2, ENABLE);					//Enable SPI2
}

void GPIO_TMC4671_SPI_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure; 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	//CS
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//SCK
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;// GPIO_Mode_Out_PP;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//MISO
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IPU;//GPIO_Mode_IN_FLOATING;// GPIO_Mode_AF_PP;//
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	//MOSI
	GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;//GPIO_Mode_Out_PP;// 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*******************************************************************************
* Function Name  : 
* Description    : Sends a byte through the SPI interface and return the byte
*                  received from the SPI bus.
* Input          : byte : byte to send.
* Output         : None
* Return         : The value of the received byte.
*******************************************************************************/
u8 SPI_TMC4671_SendByte(u8 byte)
{
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET)
		;
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI2, byte);
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
		;
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI2);
}

void SpiWrite(unsigned char spibuf)
{
	unsigned char i;
	for (i = 0; i < 8; i++) // Setup byte circulation bits
	{
		if (spibuf & 0x80)
			Set_SPI_MOSI();
		else
			ReSet_SPI_MOSI();
		delay_TMC4671(2);
		Set_SPI_SCK(); // Set clock line high
		spibuf = spibuf << 1;
		delay_TMC4671(2);
		ReSet_SPI_SCK(); // Set clock line low
	}
}

unsigned char SpiRead(void)
{
	unsigned char i, spibuf = 0;
	for (i = 0; i < 8; i++) // Setup byte circulation bits
	{
		spibuf = spibuf << 1;
		Set_SPI_SCK(); // Set clock line high
		delay_TMC4671(2);
		spibuf = spibuf | Get_SPI_MISO(); // Right shift DATA_BUF

		ReSet_SPI_SCK(); // Set clock line low
	}
	return spibuf; // Return function parameter
}

void delay_TMC4671(uint16_t dt)
{
	uint16_t m;
	m = dt * 50;
	while (m--)
	{
		// nop();
	}
}

void init_TMC4671(void)
{
	GPIO_TMC4671_SPI_init();
	SPI_TMC4671_Init();
}

void write_TMC4671(uint8_t addr,uint32_t dt)
{
	ReSet_SPI_TMC4671();
	delay_TMC4671(3);
	SPI_TMC4671_SendByte(addr);
	SPI_TMC4671_SendByte((dt >> 24) & 0xff);
	SPI_TMC4671_SendByte((dt >> 16) & 0xff);
	SPI_TMC4671_SendByte((dt >> 8) & 0xff);
	SPI_TMC4671_SendByte((dt)&0xff);
	//	SPI_TMC4671_SendByte(con);
	Set_SPI_TMC4671();
}

uint32_t Read_TMC4671(uint8_t addr)
{
	uint32_t dt;
	ReSet_SPI_TMC4671();
	delay_TMC4671(3);
	SPI_TMC4671_SendByte(addr & 0x7f);
	dt = SPI_TMC4671_SendByte(0);
	dt = (dt << 8) + SPI_TMC4671_SendByte(0);
	dt = (dt << 8) + SPI_TMC4671_SendByte(0);
	dt = (dt << 8) + SPI_TMC4671_SendByte(0);

	Set_SPI_TMC4671();
	return dt;
}

void tmc4671_writeInt(uint8_t fg, uint8_t addr, uint32_t dt)
{
	if (fg != 0)
		return;
	write_TMC4671(addr | 0x80, dt);
}

uint32_t tmc4671_readInt(uint8_t fg, uint8_t addr, uint32_t dt)
{
	if (fg != 1)
		return 0;
	dt = Read_TMC4671(addr & 0x7f);
	return dt;
}

void tmc4671_init_set(void)
{
	tmc4671_writeInt(0, TMC4671_MOTOR_TYPE_N_POLE_PAIRS, 0x00030001);
	tmc4671_writeInt(0, TMC4671_PWM_SV_CHOP, 0x00000107);
	tmc4671_writeInt(0, TMC4671_ADC_I_SELECT, 0x24000100);
	tmc4671_writeInt(0, TMC4671_dsADC_MCFG_B_MCFG_A, 0x00100010);
	tmc4671_writeInt(0, TMC4671_PWM_POLARITIES, 0x00000000);
	tmc4671_writeInt(0, TMC4671_PWM_MAXCNT, 0x000007CF);
	tmc4671_writeInt(0, TMC4671_PWM_BBM_H_BBM_L, 0x00003232);
	tmc4671_writeInt(0, TMC4671_dsADC_MCLK_A, 0x20000000);
	tmc4671_writeInt(0, TMC4671_dsADC_MCLK_B, 0x20000000);
	tmc4671_writeInt(0, TMC4671_HALL_MODE, 0x00000100);
	tmc4671_writeInt(0, TMC4671_HALL_PHI_E_PHI_M_OFFSET, 0x00000000);	//HT V2 0xC5680000
	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000005);
	tmc4671_writeInt(0, TMC4671_VELOCITY_SELECTION, 0x0000000C);
	tmc4671_writeInt(0, TMC4671_dsADC_MDEC_B_MDEC_A, 0x014E014E);
	tmc4671_writeInt(0, TMC4671_ADC_I0_SCALE_OFFSET, 0x010080D9);
	tmc4671_writeInt(0, TMC4671_ADC_I1_SCALE_OFFSET, 0x010081F0);
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 0x000003E8);
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_P_TORQUE_I, 0x01000100);
	tmc4671_writeInt(0, TMC4671_PID_FLUX_P_FLUX_I, 0x01000100);
	tmc4671_writeInt(0, TMC4671_PHI_E_SELECTION, 0x00000005);
	tmc4671_writeInt(0, TMC4671_VELOCITY_SELECTION, 0x00000005);
	tmc4671_writeInt(0, TMC4671_POSITION_SELECTION, 0x00000005);
	tmc4671_writeInt(0, TMC4671_MODE_RAMP_MODE_MOTION, 0x00000002);
	tmc4671_writeInt(0, TMC4671_ADC_I_SELECT, 0x24000100);
	
//			tmc4671_writeInt(0, 0x5c, 32767);
	tmc4671_writeInt(0, TMC4671_PIDOUT_UQ_UD_LIMITS, 32767);
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_LIMITS, 10000);
	tmc4671_writeInt(0, TMC4671_PID_ACCELERATION_LIMITS, 400000);
	tmc4671_writeInt(0, TMC4671_PID_VELOCITY_LIMIT, 70000);
	tmc4671_writeInt(0, TMC4671_PID_POSITION_LIMIT_LOW, -2147483647);
	tmc4671_writeInt(0, TMC4671_PID_POSITION_LIMIT_HIGH, 2147483647);
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_P_TORQUE_I, (PID_TORQUE_P<<16)|PID_TORQUE_I);
	tmc4671_writeInt(0, TMC4671_PID_FLUX_P_FLUX_I, (PID_FLUX_P<<16)|PID_FLUX_I);
	tmc4671_writeInt(0, TMC4671_PID_VELOCITY_P_VELOCITY_I, (PID_VELOCITY_P<<16)|PID_VELOCITY_I);
	tmc4671_writeInt(0, TMC4671_PID_POSITION_P_POSITION_I, (PID_POSITION_P<<16)|PID_POSITION_I);
	tmc4671_writeInt(0, TMC4671_PID_TORQUE_FLUX_TARGET, 0x00000000);
}
