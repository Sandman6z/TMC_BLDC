# TMC_BLDC

* 基于STM32F103 & TMC4671方案的无刷电机驱动  


## 2023.09.01
``` STM32F103_BLDC_瑞士涡轮-增加过欠压保护-适配v2.00电路板+风扇 -调试最大转速 ```

## 2023.09.19
```	
	PA2 -> MOS_T_ADC(PC4)
	PA3	-> I_SUM_ADC(PC5)
	PB0	PWBUS_ADC -> I_SET
	PB4 BRAKE -> BRAKE_EN
	PB7 STAT_OUT -> READY
	PB9 FAN_CTRL -> WORK_LED

