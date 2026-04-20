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

/*------------------------------------------variables-------------------------------------------*/
__IO uint8_t g_timestamp_flag = 0;

/*------------------------------------------functions-------------------------------------------*/
int main(void)
{
    uint8_t ts_time[3], ts_date[2];

    /* 配置系统时钟 */
    system_clock_config(); 

    /* UART GPIO初始化 */
    gpio_init();

    /* UART初始化 */
    uart_init();

    /* RTC时钟源选择LXTAL */
    rtc_clock_config();
    
    /* RTC在VCORE_AON域，可使用VCORE_AON复位清除所有寄存器状态 */

    /* RTC日期时间配置 */
    bsp_rtc_datetime_config();
    
    /* TAMP IN入侵检测配置 */
    bsp_rtc_tamp_in_config();
    
    /* 等待TAMP IN外部入侵事件中断产生 */
    while (g_timestamp_flag == 0);

    /* 获取时间戳日期与时间 */
    bsp_rtc_get_timestamp(ts_date, ts_time); 
    
    /* 串口打印时间戳时间，显示格式: hh:ms:ss */
    printf("Timestamp Time = %02d:%02d:%02d\r\n",  
                           std_rtc_convert_bcd2bin(ts_time[0]), 
                           std_rtc_convert_bcd2bin(ts_time[1]), 
                           std_rtc_convert_bcd2bin(ts_time[2]));
        
    /* 串口打印时间戳时间，显示格式: dd-mm */
    printf("Timestamp Date = %2d-%2d\r\n",  
                           std_rtc_convert_bcd2bin(ts_date[0]), 
                           std_rtc_convert_bcd2bin(ts_date[1]));

    while(1)
    {
    }
}
