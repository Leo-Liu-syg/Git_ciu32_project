/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              应用程序主流程。
*                     
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "main.h"

/*--------------------------------------------variables-----------------------------------------*/
__IO uint8_t g_calib_flag = 0;

/*--------------------------------------------define--------------------------------------------*/
/* 时钟精度ppm值，根据实测RTC时钟精度填写 */
#define FRTC_PPM (-20L)

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    float calp_val = 0, calm_val = 0;
    
    /* 配置系统时钟 */
    system_clock_config(); 

    /* Systick初始化 */
    std_delay_init();

    /* RTC时钟源选择LXTAL */
    rtc_clock_config();

    /* RTC在VCORE_AON域，可使用VCORE_AON复位清除所有寄存器状态 */
    
    /* 关闭RTC寄存器写保护 */
    std_rtc_write_protection_disable();  

    /* RTC_OUT输出预分频的时钟信号1Hz */
    bsp_rtc_output_config();

    /* 等待3s */
    std_delayms(3000); 

    if(FRTC_PPM > 0)
    {
        calp_val = RTC_SMOOTH_CALIB_PLUS_PULSES_RESET;
        calm_val = FRTC_PPM/0.954 + 0.5;
    }
    else if(FRTC_PPM < 0)
    {
        calp_val = RTC_SMOOTH_CALIB_PLUS_PULSES_SET;
        calm_val = (512 + (FRTC_PPM/0.954) + 0.5);
    }

    /* 设置校准参数 */
    bsp_rtc_calib_config((uint32_t)calp_val, (uint32_t)calm_val);

    /* 使能RTC寄存器写保护 */
    std_rtc_write_protection_enable();
    
    /* 等待校准周期中断 */    
    while(g_calib_flag == 0)
    {
        /* 校准周期结束后处理代码 */
    }
    
    while(1)
    {
    }
}
