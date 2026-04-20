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
    uint8_t showtime[3] = {0};
    uint32_t hours = 0;
    uint32_t minutes = 0;
    uint32_t seconds = 0;
    
    /* 配置系统时钟 */
    system_clock_config();
    
    /* Systick初始化 */
    std_delay_init();
    
    led_init();
    
    /* 配置按键，作为stop唤醒源 */    
    exti_init();    
    
    /* UART GPIO初始化 */
    gpio_init();

    /* UART初始化，并使能 */
    uart_init();    
    std_usart_enable(USART1);
    
    /* RTC初始化，RTC时钟源为LXTAL */
    rtc_clock_config();
                
    /* 等待3S，便于用户调试 */
    std_delayms(3000); 
        
    /* 获取当前时间 */
    bsp_rtc_show_time(showtime);
    hours = std_rtc_convert_bcd2bin(showtime[0]);
    minutes = std_rtc_convert_bcd2bin(showtime[1]);
    seconds = std_rtc_convert_bcd2bin(showtime[2]);
    
    /* 串口打印当前时间，显示时间格式 : hh:mm:ss */
    printf("Current Time = %02d:%02d:%02d\r\n", hours, minutes, seconds);    
    
    /* 使能超低功耗配置 */
    bsp_pmu_lowpower_config();
    
    /* 进入Stop模式 */
    LED1_OFF();     
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);
    
    /* 等待按键唤醒Stop */
    
    
    /* Stop唤醒后，恢复系统时钟配置 */
    LED1_ON();      
    system_clock_config(); 
    
    bsp_rtc_show_time(showtime);
    hours = std_rtc_convert_bcd2bin(showtime[0]);
    minutes = std_rtc_convert_bcd2bin(showtime[1]);
    seconds = std_rtc_convert_bcd2bin(showtime[2]);
    
    /* 串口打印当前时间，显示时间格式 : hh:mm:ss */
    printf("Current Time = %02d:%02d:%02d\r\n", hours, minutes, seconds);    
 
    while(1)
    {
       
    }
}




