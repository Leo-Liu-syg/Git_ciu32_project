/************************************************************************************************/
/**
* @file               pmu_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              PMU BSP驱动函数，实现PMU Stop相关功能配置。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "pmu_bsp.h"

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  GPIO_PIN_4到GPIO_PIN_15中断服务函数
* @retval 无
*/
void EXTI4_15_IRQHandler(void)
{
    /* 检测EXTI Line 中断 */
    if (std_exti_get_pending_status(EXTI_LINE_GPIO_PIN13))
    {
        std_exti_clear_pending(EXTI_LINE_GPIO_PIN13);
    }
    
    /* 用户根据EXTI的中断事件，结合当前的状态实现中断处理函数 */    
}


/**
* @brief  使能PMU低功耗配置
* @retval 无
*/
void bsp_pmu_lowpower_config(void)
{
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    
    /* 使能超低功耗配置 */
    std_pmu_ultra_lowpower_enable();
}


