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
uint8_t g_match_charactor = 0x03;
/*-------------------------------------------functions------------------------------------------*/

/**
* @brief  LPUART中断服务函数
* @retval 无
*/
void LPUART1_IRQHandler(void)
{
    /* 判断中断来自字符匹配标志 */
    if(((std_lpuart_cr1_get_interrupt_enable(LPUART1,LPUART_CR1_INTERRUPT_CM))
         &&(std_lpuart_get_flag(LPUART1,LPUART_FLAG_CM))) != 0)
    {
        /* 标志位清零， 中断关闭 */
        std_lpuart_clear_flag(LPUART1,LPUART_FLAG_CM); 
        std_lpuart_cr1_interrupt_disable(LPUART1,LPUART_CR1_INTERRUPT_CM);
        
        /* 获取已接收字符 */
        g_temp_rx = (std_lpuart_rx_data_read(LPUART1) & 0xFF);
    }
 
}

/**
* @brief  LPUART 从Stop 唤醒配置
* @note   接收到匹配字符唤醒
* @retval 无
*/
void bsp_lpuart_wakeup_config(void)
{
    std_lpuart_disable(LPUART1);
    
    /* LPUART 配置为接收到匹配字符唤醒 */
    std_lpuart_set_data_match(LPUART1, (uint32_t)g_match_charactor);
   
    /* LPUART 从Stop模式唤醒使能 */
    std_lpuart_wakeup_enable(LPUART1);
   
    /* 使能低功耗唤醒中断 */
    std_lpuart_clear_flag(LPUART1,LPUART_CLEAR_CM);    
    std_lpuart_cr1_interrupt_enable(LPUART1,LPUART_CR1_INTERRUPT_CM);
    
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

/**
* @brief  进入低功耗前清零相关标志
* @note   接收到匹配字符唤醒
* @retval 无
*/
void bsp_lpuart_prepare_to_stop(void)
{
    std_lpuart_request_rx_flush(LPUART1);
    std_lpuart_clear_flag(LPUART1,LPUART_FLAG_ORE);
}
