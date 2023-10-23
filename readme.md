# TMC_BLDC

* 基于STM32F103 & TMC4671方案的无刷电机驱动  


## 2023.09.01
``` STM32F103_BLDC_瑞士涡轮-增加过欠压保护-适配v2.00电路板+风扇 -调试最大转速 ```

## 2023.09.19
```	
! NEED CHANGE PORT !
	MOS_T_ADC	PC4(IN14) -> PA2(IN2)
	I_SUM_ADC	PC5(IN15) -> PA3(IN3)
```
```
ONLY CHANGE NAME:
	PB0	PWBUS_ADC	-> I_SET
	PB4	BRAKE 		-> BRAKE_EN
	PB7	STAT_OUT 	-> READY
	PB9	FAN_CTRL 	-> WORK_LED
```

## 2023.10.17
	!!! TURBO START TURNING !!!
	change TMC4671_ADC_I_SELECT from 0x24000100 to 0x06000100


## 2023.10.23
| ADC_Channel | No. | GPIO | old name | new name | new ADC_Channel |
|:-:|:-:|:-:|:-:|:-:|:-:|
| ADC_Channel_0 | 1 | PA0 | **BRAKE_TEST** | **BRAKE_DETECT** |  |
| ADC_Channel_1 | 2 | PA1 | AD_SPEED   | AD_SPEED     |  |
| ADC_Channel_5 | 3 | PA5 | TUR_T_ADC  | TUR_T_ADC    |  |
| ADC_Channel_4 | 4 | PA4 | VM_ADC     | VM_ADC       |  |
| ADC_Channel_8 | 5 | PB0 | PWBUS_ADC  |              |  |
||||||||
|               |   | PA2 |            | MOS_T_ADC    | ADC_Channel_2 |
|               |   | PA3 |            | I_SUM_ADC    | ADC_Channel_3 |

|||
| :-: | :-: |
BRAKE_DETECT |	PA0
AD_SPEED	 |	PA1
MOS_T_ADC	 |	PA2
I_SUM_ADC	 |	PA3
VM_ADC		 |	PA4
TUR_T_ADC	 |	PA5

