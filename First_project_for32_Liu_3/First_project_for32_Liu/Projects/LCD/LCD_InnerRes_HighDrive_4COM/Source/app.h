#ifndef __APP_H_
#define __APP_H_

#include "main.h"

// 32位无符号整数绝对值计算
#define ABS_DIFF_U32(a, b) (((a) > (b)) ? ((a) - (b)) : ((b) - (a)))

#define Col_Address 0
#define Vol_Zero_Address 4

// 外部变量声明
extern __IO uint32_t Get_CO_Voltage;     // ADC通道3 PA4采集的CO电压
extern __IO uint32_t Get_Smoke_Voltage;  // ADC通道1 PA2采集的烟雾电压
extern __IO uint32_t Get_CO_Zero;        // CO传感器零点电压
extern __IO uint32_t Get_Smoke_Zero;     // 烟雾传感器零点电压
extern unsigned char Running_LCD_Flag;
extern unsigned int  adc_co_calibration_value;
extern unsigned char Running_LCD_counter; // LCD倒计时

// 应用状态机枚举
typedef enum
{
    Status_IDLE = 0,        // 初始空闲状态，未开始运行
    Status_SELF_CHECK,      // 系统自检状态（硬件/ADC/传感器检测）
    Status_CAL,             // 校准状态，校准完成后进入正常运行
    Status_RUNNING,         // 正常运行状态，实时采集、转换、显示
    Status_ERROR            // 故障状态，自检/校准失败时进入
} App_status;

extern App_status Status;

// 应用层函数声明
void app_get_adc_data_process(void);
void app_Status_Control(void);
void app_value_convert(void);
void app_LED_Control(void);
void app_LCD_Control(void);

#endif // __APP_H_