#ifndef __BSP_MONITOR_H__
#define __BSP_MONITOR_H__

void wdg_init(void);
void get_speed(void);
float Calculate_temperature(uint32_t adc_V, float Bx);
void MOS_TempCheck(void);
void BUS_Voltage_Calc(void);
void PowerCheck(void);
void Overvoltage_oprate(void);
void ResExistDetect(void);
void SysInit(void);
int inverseMapADCValue(uint16_t adc_value);


#endif /* __BSP_MONITOR_H__ */
