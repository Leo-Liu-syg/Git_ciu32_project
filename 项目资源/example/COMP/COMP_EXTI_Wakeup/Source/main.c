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

/*------------------------------------------functions-------------------------------------------*/
int main(void)
{
    /* 配置系统时钟为RCH 2分频（8MHz）*/
    system_clock_config();

    /* 延时函数初始化 */
    std_delay_init();
    
    /* GPIO初始化 */
    gpio_init();

    /* VBGR初始化 */
    vbgr_init();

    /* COMP1初始化 */
    comp1_init();

    /* NVIC初始化 */
    nvic_init(); 

    /* EXTI初始化 */
    exti_init();

    /* 进入Stop前延时3s便于debug，正常使用不用延时 */
    std_delayms(3000);

    /* 使能超低功耗模式 */    
    std_pmu_ultra_lowpower_enable();
    /* 进入Stop模式 */
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);

    /* 唤醒后, 维持系统时钟为8MHz不变 */    

    while(1)
    {
        /* 唤醒后LED1闪烁 */
        LED_RED_FLIP();
        std_delayms(300); 
    }
}


