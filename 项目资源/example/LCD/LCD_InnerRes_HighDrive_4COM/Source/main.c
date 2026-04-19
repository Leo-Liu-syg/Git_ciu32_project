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
__IO uint8_t g_button_pushed = 0;            /* 按键按下标志     */
__IO uint8_t g_button_count = 0;             /* 按键按下次数计数 */
__IO uint8_t g_lcd_stop = 0;                 /* 进入Stop 标志    */
/*------------------------------------------functions-------------------------------------------*/
int main(void)
{
    /* 配置系统时钟为RCH 2分频（8MHz）*/
    system_clock_config();

    /* Systick初始化 */
    std_delay_init();
    
    /* EXTI初始化 */   
    exti_init();

    /* LED1初始化 */
    led_init();
    
    /* LCD用GPIO配置 */
    gpio_init();
    /* LCD时钟配置 */
    lcd_clock_config();
    /* LCD初始化 */
    lcd_init();

    bsp_lcd_display_string((uint8_t*)"1234");

    /* 进入Stop前delay 3s */
    std_delayms(3000);
   
    /* 使能超低功耗模式 */    
    std_pmu_ultra_lowpower_enable();   
    /* 进入Stop模式 */  
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);

    /* 唤醒后,系统时钟仍为8MHz */

    while(1)
    {
        /* 闪烁LED1 */
        LED1_TOGGLE(); 
        std_delayms(300);   
    }
}


