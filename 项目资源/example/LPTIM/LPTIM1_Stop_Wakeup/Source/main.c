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
    /* 设置RCHSYS作为系统时钟源：RCH的2分频，8MHz */
    system_clock_config();
    
    /* Systick初始化 */
    std_delay_init();
    
    /* GPIO初始化 */
    gpio_init();
    
    /* 设置EXTI中LPTIM1时间唤醒使能 */
    exti_init();
    
    /* LPTIM1初始化 */
    lptim1_init();
    
    /* LPTIM1 使能timeout功能 */
    bsp_lptim_timeout_enable();
    
    /* 点亮LED1 */
    LED1_ON();
    
    /* 设置MCU进入低功耗Stop模式 */
    bsp_pmu_lowpower_config();
    std_delayms(3000);
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFE);
    
    /* 唤醒后，系统时钟为8MHz，即RCH的2分频 */
    
    /* 熄灭LED1 */
    LED1_OFF();

    /* LPTIM 禁止timeout功能 */
    bsp_lptim_timeout_disable();
    
    while(1)
    {
    }
}

