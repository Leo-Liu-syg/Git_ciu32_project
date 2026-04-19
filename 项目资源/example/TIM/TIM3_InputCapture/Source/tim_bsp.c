/************************************************************************************************/
/**
* @file               tim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              TIM BSP驱动函数,实现TIM中断方式输入捕获功能
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/


/*------------------------------------------includes--------------------------------------------*/
#include "tim_bsp.h"

/*--------------------------------------------variables-----------------------------------------*/
__IO uint32_t g_ch2_ccx_value = 0U;


/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  TIM3中断处理程序
* @retval 无
*/
void TIM3_IRQHandler(void)
{
    /* CC2中断处理流程 */
    if ((std_tim_get_flag(TIM3, TIM_FLAG_CC2)) && (std_tim_get_interrupt_enable(TIM3, TIM_INTERRUPT_CC2)))
    {
        /* 关闭CC2中断 */
        std_tim_interrupt_disable(TIM3, TIM_INTERRUPT_CC2);
        g_ch2_ccx_value = std_tim_get_ccx_value(TIM3, TIM_CHANNEL_2);
        
        /* 用户对捕获值处理... */
        
    }    
}

/**
* @brief  TIM3启动计数，并使能捕获事件中断
* @retval 无
*/
void bsp_tim3_capture_start(void)
{
    /* 使能TIM3输入捕获通道2中断 */
    std_tim_interrupt_enable(TIM3, TIM_INTERRUPT_CC2);
    
    /* 使能输入捕获 */
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_2);
    
    /* 启动TIM3计数 */
    std_tim_enable(TIM3);

}
