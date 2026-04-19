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
__IO uint8_t g_wut_sec_flag = 0;
__IO uint8_t g_wut_2hz_flag = 0;

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    uint8_t rtctime[3], rtcdate[3];
    
    /* 设置RCHSYS作为系统时钟源: RCH的2分频，8MHz */
    system_clock_config();

    /* Systick初始化 */
    std_delay_init();

    /* LED1初始化 */
    led_init();

    /* UART GPIO初始化 */
    gpio_init();

    /* UART初始化 */
    uart_init();

    /* RTC时钟源选择LXTAL */
    rtc_clock_config();
    
    /* RTC在VCORE_AON域，可使用VCORE_AON复位清除所有寄存器状态 */

    /* RTC日期时间配置 */
    bsp_rtc_datetime_config();

    /* RTC唤醒定时器配置 */
    bsp_rtc_wakeup_config();
    
    /* 等待3S */
    std_delayms(3000); 

    /* 关闭LED1 */
    LED1_OFF();

    /* 使能超低功耗配置 */
    bsp_pmu_lowpower_config();

    /* 进入stop模式 */  
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);

    /* 唤醒后,系统时钟仍为8MHz，即RCH的2分频 */
    
    while(1)
    {
        /* 等待周期中断 */
        while((g_wut_2hz_flag == 0) && (g_wut_sec_flag == 0)) 
        {
        }

        if(g_wut_2hz_flag)
        {
            /* LED1闪烁 */
            LED1_TOGGLE();
            
            g_wut_2hz_flag = 0;
        }
        
        if(g_wut_sec_flag)
        {
            /* 读取日历时间 */
            bsp_rtc_get_calendar(rtcdate, rtctime);
            /* 串口打印日历时间，显示格式: hh:ms:ss */
            printf("Current Time = %02d:%02d:%02d\r\n",  
                               std_rtc_convert_bcd2bin(rtctime[0]), 
                               std_rtc_convert_bcd2bin(rtctime[1]), 
                               std_rtc_convert_bcd2bin(rtctime[2]));

            g_wut_sec_flag = 0;
        }
    }
}
