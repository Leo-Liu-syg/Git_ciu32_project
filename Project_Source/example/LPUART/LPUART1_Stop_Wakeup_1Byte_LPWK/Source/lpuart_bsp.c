/************************************************************************************************/
/**
* @file               lpuart_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              LPUART BSP驱动函数，实现LPUART功能配置
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "lpuart_bsp.h"

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  LPUART中断服务函数
* @retval 无
*/
void LPUART1_IRQHandler(void)
{
    /* 检查唤醒中断 */
    if(((std_lpuart_cr3_get_interrupt_enable(LPUART1,LPUART_CR3_INTERRUPT_LPWK))
         &&(std_lpuart_get_flag(LPUART1,LPUART_FLAG_LPWKF))) != 0)
    {
        /* 唤醒后关闭唤醒中断 */
        std_lpuart_cr3_interrupt_disable(LPUART1,LPUART_CR3_INTERRUPT_LPWK);
        /* 关闭LPUART唤醒功能,关闭后LPWK标志同步清零 */
        std_lpuart_wakeup_disable(LPUART1);
    }
}

/**
* @brief  LPUART 从Stop 唤醒配置
* @note   使用LPWK标志唤醒，触发条件为接收一帧数据
* @retval 无
*/
void bsp_lpuart_wakeup_config(void)
{
    std_lpuart_disable(LPUART1);
    
    /* LPUART 选择接收LPWK分支接收一帧数据唤醒 */
    std_lpuart_set_wakeup_method(LPUART1,LPUART_WAKEUP_ON_READDATA_NOTEMPTY);
    /* LPUART 从Stop模式唤醒使能 */
    std_lpuart_wakeup_enable(LPUART1);
    /* 使能低功耗唤醒中断 */
    std_lpuart_clear_flag(LPUART1,LPUART_FLAG_LPWKF);    
    std_lpuart_cr3_interrupt_enable(LPUART1,LPUART_CR3_INTERRUPT_LPWK);
    
    std_lpuart_enable(LPUART1);
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

