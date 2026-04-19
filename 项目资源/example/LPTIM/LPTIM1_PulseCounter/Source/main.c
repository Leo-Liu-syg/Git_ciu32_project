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
    /* 设置RCH的2分频作为系统时钟源：8MHz */
    system_clock_config();
    
    /* Systick初始化 */
    std_delay_init();
    
    /* GPIO初始化 */
    gpio_init();
    
    /* LPTIM1初始化 */
    lptim1_init();
    
    /* NVIC初始化 */
    nvic_init();
    
    /* 点亮LED1 */
    LED1_ON();
    
    /* LPTIM使能自动重载匹配中断后启动计数 */
    bsp_lptim_counter_start();
    
    /* 设置MCU进入低功耗Stop模式 */
    bsp_pmu_lowpower_config();
    std_delayms(3000);
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);
    
    /* 唤醒后,系统时钟为8MHz，即RCH的2分频 */
    
    while(1)
    {
        /* LED1闪烁 */
        std_delayms(200);
        LED_RED_FLIP();
    }
}

