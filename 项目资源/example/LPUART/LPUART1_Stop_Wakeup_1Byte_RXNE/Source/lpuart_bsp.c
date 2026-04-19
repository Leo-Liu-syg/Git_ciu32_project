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
__IO uint8_t g_temp_rx = 0;       /* 全局接收缓冲区 */
/*-------------------------------------------functions------------------------------------------*/

/**
* @brief  LPUART中断服务函数
* @retval 无
*/
void LPUART1_IRQHandler(void)
{
    /* 检查上溢错误，RXNE或ERR中断使能均可产生ORE中断*/
    if((std_lpuart_cr1_get_interrupt_enable(LPUART1,LPUART_CR1_INTERRUPT_RXNE)      
         || std_lpuart_cr3_get_interrupt_enable(LPUART1,LPUART_CR3_INTERRUPT_ERR))  
         && std_lpuart_get_flag(LPUART1,LPUART_FLAG_ORE))                           
    {
        /* 清除上溢错误标志 */
        std_lpuart_clear_flag(LPUART1,LPUART_CLEAR_ORE);
        /* 用户可根据实际使用场景，自定义错误处理流程 */  
    }
    /* 产生接收一帧数据中断标志时，读取接收数据 */
    if(((std_lpuart_cr1_get_interrupt_enable(LPUART1,LPUART_CR1_INTERRUPT_RXNE))
         &&(std_lpuart_get_flag(LPUART1,LPUART_FLAG_RXNE))) != 0)
    {
        g_temp_rx = std_lpuart_rx_data_read(LPUART1); 
    }
}

/**
* @brief  LPUART 从Stop 唤醒配置
* @note   接收到一帧数据唤醒
* @retval 无
*/
void bsp_lpuart_wakeup_config(void)
{
    /* 通过使能RXFNE中断，实现接收到一帧数据唤醒 */
    std_lpuart_cr1_interrupt_enable(LPUART1,LPUART_CR1_INTERRUPT_RXNE);
    
    /* LPUART 从Stop模式唤醒使能 */
    std_lpuart_wakeup_enable(LPUART1);
    
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
