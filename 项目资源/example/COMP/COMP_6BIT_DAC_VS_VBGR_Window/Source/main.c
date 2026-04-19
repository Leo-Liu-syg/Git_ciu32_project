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
    uint32_t voltage_state = 0U;

    /* 配置系统时钟 */
    system_clock_config();
    
    /* 延时函数初始化 */
    std_delay_init();
    
    /* GPIO初始化 */
    gpio_init();

    /* VBGR初始化 */
    vbgr_init();

    /* 6BIT DAC初始化 */
    syscfg_6bit_dac_init();
    
    /* COMP1和COMP2初始化为窗口比较器 */
    comp_init();
    
    while(1)
    {
        /* 检查输入电压 */
        voltage_state = bsp_input_voltage_level_check();
        if((voltage_state == STATE_OVER_THRESHOLD) || (voltage_state == STATE_UNDER_THRESHOLD))
        {
            /* 超过阈值范围点亮LED1 */
            LED1_ON();
        }
        else
        {
            /* 在阈值范围内熄灭LED1 */
            LED1_OFF();
        }
    }
}

