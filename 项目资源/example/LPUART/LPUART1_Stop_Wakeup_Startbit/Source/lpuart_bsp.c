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
/*------------------------------------------variables-------------------------------------------*/
__IO uint8_t g_temp_rx = 0;
/*-------------------------------------------functions------------------------------------------*/

/**
* @brief  LPUART中断服务函数
* @retval 无
*/
void LPUART1_IRQHandler(void)
{
    /* 等待唤醒中断 */
    if(((std_lpuart_cr3_get_interrupt_enable(LPUART1,LPUART_CR3_INTERRUPT_LPWK))
         &&(std_lpuart_get_flag(LPUART1,LPUART_FLAG_LPWKF))) != 0)
    {
        /* 关闭唤醒中断，清零唤醒标志位 */
        std_lpuart_cr3_interrupt_disable(LPUART1,LPUART_CR3_INTERRUPT_LPWK);
        std_lpuart_clear_flag(LPUART1,LPUART_FLAG_LPWKF);
        
        /* 等待接收非空标志置1后，获取接收数据*/
        while(!std_lpuart_get_flag(LPUART1,LPUART_FLAG_RXNE))
        {
        }
        g_temp_rx = (std_lpuart_rx_data_read(LPUART1) & 0xFF);  
    }
}

/**
* @brief  LPUART 从Stop 唤醒配置
* @note   接收到起始位唤醒
* @retval 无
*/
void bsp_lpuart_wakeup_config(void)
{
    std_lpuart_disable(LPUART1);
    
    /* LPUART 选择接收到起始位唤醒 */
    std_lpuart_set_wakeup_method(LPUART1,LPUART_WAKEUP_ON_STARTBIT);
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
