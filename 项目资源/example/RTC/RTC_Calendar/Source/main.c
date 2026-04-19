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

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    uint8_t rtctime[3], rtcdate[3];
    
    /* 配置系统时钟 */
    system_clock_config(); 

    /* Systick初始化 */
    std_delay_init();

    /* UART GPIO初始化 */
    gpio_init();

    /* UART初始化 */
    uart_init();

    /* RTC时钟源选择LXTAL */
    rtc_clock_config();
    
    /* RTC在VCORE_AON域，可使用VCORE_AON复位清除所有寄存器状态 */

    /* RTC日期时间配置 */
    bsp_rtc_datetime_config();

    /* RTC_OUT输出预分频的时钟信号1Hz */
    bsp_rtc_output_config();
    
    while(1)
    {
        /* 获取日历日期与时间 */
        bsp_rtc_get_calendar(rtcdate, rtctime); 
        
        /* 串口打印日历时间，显示格式: hh:ms:ss */
        printf("Current Time = %02d:%02d:%02d\r\n",  
                               std_rtc_convert_bcd2bin(rtctime[0]), 
                               std_rtc_convert_bcd2bin(rtctime[1]), 
                               std_rtc_convert_bcd2bin(rtctime[2]));
            
        /* 串口打印日历时间，显示格式: dd-mm-yyyy */
        printf("Current Date = %2d-%2d-%2d\r\n",  
                               std_rtc_convert_bcd2bin(rtcdate[2]), 
                               std_rtc_convert_bcd2bin(rtcdate[1]), 
                               2000 + std_rtc_convert_bcd2bin(rtcdate[0]));
        
        std_delayms(1000);
    }  
}
