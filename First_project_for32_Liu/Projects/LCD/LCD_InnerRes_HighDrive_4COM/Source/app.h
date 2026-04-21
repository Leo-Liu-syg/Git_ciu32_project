#ifndef __APP_H_
#define __APP_H_
#include "main.h"
// 取32位无符号数绝对值
#define ABS_DIFF_U32(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
extern __IO uint32_t Get_CO_Voltage;    // adc-??3-PA4????
extern __IO uint32_t Get_Smoke_Voltage; // adc-??1
extern __IO uint32_t Get_CO_Zero;    // adc-??3-PA4????
extern __IO uint32_t Get_Smoke_Zero; // adc-??1-
extern unsigned char Running_LCD_Flag;

void app_get_adc_data_process(void);
void app_Status_Control(void); 
void app_value_convert(void);
void app_LED_Control(void);
void app_LCD_Control(void);

#endif // __APP_H__