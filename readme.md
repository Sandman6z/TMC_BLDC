# TMC_BLDC

* 基于STM32F103 & TMC4671方案的无刷电机驱动  


***写在前面***
> > > > > >
-	后来接手的朋友请注意，配置代码写的到处都是，本人水平有限，到手后只重构了工程路径结构，函数格式化、增加了readme、changelog，采用git管理代码而已。像main函数中的很多系统配置和自定义函数，看不懂，不敢动。这是已经分出去很多gpio.c、led.c等文件了。
-	且自修改之日起，新项目应逐渐转用STM32 HAL库，这是官方强推的主流，且新出的单片机官方仅支持HAL库，早晚都要转，不如趁早用起来。

望能遇到大佬，将本项目重新梳理，并用HAL实现。

---

## 2023.09.01 [^1]
[^1]:STM32F103_BLDC_瑞士涡轮-增加过欠压保护-适配v2.00电路板+风扇 -调试最大转速

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

