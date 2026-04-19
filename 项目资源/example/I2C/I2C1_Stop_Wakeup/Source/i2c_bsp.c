/************************************************************************************************/
/**
* @file               i2c_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              I2C BSP驱动函数，实现I2C低功耗STOP模式下唤醒等函数。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "i2c_bsp.h"

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  I2C1中断服务程序
* @retval 无
*/
void I2C1_IRQHandler(void)
{
    /* ADDRIE中断处理流程 */
    if (std_i2c_get_interrupt_enable(I2C1, I2C_INTERRUPT_ADDR) && std_i2c_get_flag(I2C1, I2C_FLAG_ADDR))
    {
        /* 清除地址匹配中断标志位 */
        std_i2c_clear_flag(I2C1, I2C_FLAG_ADDR);
        
        /* LED1常亮 */
        LED1_ON();
    }
}

/**
* @brief  低功耗模式配置
* @retval 无
*/
void bsp_lowpower_config(void)
{
    /* 使能PMU RCC时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    
    /* 使能超低功耗配置 */
    std_pmu_ultra_lowpower_enable();
    
    /* 配置RCH 2分频 */
    std_rcc_set_rchdiv(RCC_RCH_DIV2);
}
