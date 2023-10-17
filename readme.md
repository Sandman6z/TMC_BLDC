# TMC_BLDC

* 基于STM32F103 & TMC4671方案的无刷电机驱动  


## 2023.09.01
``` STM32F103_BLDC_瑞士涡轮-增加过欠压保护-适配v2.00电路板+风扇 -调试最大转速 ```

## 2023.09.19
```	
! NEED CHANGE PORT !
	MOS_T_ADC	PC4(IN14) -> PA2(IN2)
	I_SUM_ADC	PC5(IN15) -> PA3(IN3)

ONLY CHANGE NAME:
	PB0	PWBUS_ADC	-> I_SET
	PB4	BRAKE 		-> BRAKE_EN
	PB7	STAT_OUT 	-> READY
	PB9	FAN_CTRL 	-> WORK_LED


## 2023.10.17
	!!! TURBO START TURNING !!!
	change TMC4671_ADC_I_SELECT from 0x24000100 to 0x06000100
