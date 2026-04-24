#ifndef __APP_H_
#define __APP_H_
#include "main.h"
// 取32位无符号数绝对值
#define ABS_DIFF_U32(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))
extern __IO uint32_t Get_CO_Voltage;    // adc-??3-PA4????
extern __IO uint32_t Get_Smoke_Voltage; // adc-??1
extern __IO uint32_t Get_CO_Zero;       // adc-??3-PA4????
extern __IO uint32_t Get_Smoke_Zero;    // adc-??1-
extern unsigned char Running_LCD_Flag;
extern unsigned int adc_co_calibration_value;
extern unsigned char Running_LCD_counter; // LCD控制
typedef enum
{
    Status_IDLE = 0,   // 初始待机状态（未启动流程）
    Status_SELF_CHECK, // 传感器整体硬件自检（ADC/供电/通信）
    Status_CAL,        // 自检完成（判断是否通过）
    Status_RUNNING,    // 正常运行状态（实时采集/转换值）
    Status_ERROR       // 异常状态（自检/标定失败，需处理）
} App_status;
extern App_status Status;

void app_get_adc_data_process(void);
void app_Status_Control(void);
void app_value_convert(void);
void app_LED_Control(void);
void app_LCD_Control(void);

#endif // __APP_H__