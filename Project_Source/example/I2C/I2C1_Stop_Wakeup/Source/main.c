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
    /* 配置系统时钟 */
    system_clock_config();
    
    /* Systick初始化 */
    std_delay_init();
    
    /* I2C1和LED GPIO初始化 */
    gpio_init();
    
    /* NVIC初始化 */
    nvic_init();
    
    /* I2C1初始化为从模式 */
    i2c1_slave_init();
    
    std_delayms(3000);

    /* STOP模式配置：使能超低功耗配置，并配置唤醒后RCH的分频系数（2分频） */
    bsp_lowpower_config();
    std_pmu_enter_stop(PMU_ENTRY_LOWPOWER_MODE_WFI);
    
    /* 等待地址匹配，唤醒后系统时钟为RCH 2分频（8MHz） */
    while(1)
    {
       
    }
}




