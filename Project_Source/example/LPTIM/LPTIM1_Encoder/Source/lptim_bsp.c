/************************************************************************************************/
/**
* @file               lptim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              LPTIM BSP驱动函数，实现LPTIM 正交编码器等功能
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "lptim_bsp.h"

/*--------------------------------------------define--------------------------------------------*/
#define PERIOD          (uint32_t)(100UL)      

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  LPTIM1中断服务函数
* @retval 无
*/
void LPTIM1_IRQHandler(void)
{
    /* 自动重载匹配中断处理 */
    if ((std_lptim_get_interrupt_status(LPTIM1, LPTIM_INTERRUPT_ARRM)) && (std_lptim_get_flag(LPTIM1, LPTIM_FLAG_ARRM)))
    {
        std_lptim_clear_flag(LPTIM1, LPTIM_CLEAR_ARRM);
        
        /* 自动重载匹配的处理 */
        
    }
    /* 方向变为递增中断处理 */
    if ((std_lptim_get_interrupt_status(LPTIM1, LPTIM_INTERRUPT_UP)) && (std_lptim_get_flag(LPTIM1, LPTIM_FLAG_UP)))
    {
        std_lptim_clear_flag(LPTIM1, LPTIM_CLEAR_UP);
        
        /* 编码器计数方向变为递增的处理 */

    }
    /* 方向变为递减中断处理 */
    if ((std_lptim_get_interrupt_status(LPTIM1, LPTIM_INTERRUPT_DOWN)) && (std_lptim_get_flag(LPTIM1, LPTIM_FLAG_DOWN)))
    {
        std_lptim_clear_flag(LPTIM1, LPTIM_CLEAR_DOWN);
        
        /* 编码器计数方向变为递减的处理 */

    }
}

/**
* @brief  使能中断、正交编码器接口并启动计数
* @retval 无
*/
void bsp_lptim_encoder_start(void)
{
    /* 使能LPTIM1自动重装载中断、方向变为递增中断和方向变为递减中断 */
    std_lptim_interrupt_enable(LPTIM1, LPTIM_INTERRUPT_ARRM | LPTIM_INTERRUPT_UP | LPTIM_INTERRUPT_DOWN);
    
    /* 使能正交编码接口 */
    std_lptim_encoder_enable(LPTIM1);
    
    /* 使能LPTIM1 */
    std_lptim_enable(LPTIM1);
    
    /* 加载周期值（ARR寄存器） */
    std_lptim_set_auto_reload(LPTIM1, PERIOD);
    
    /* 以PWM方式启动计数 */
    std_lptim_start_counter(LPTIM1, LPTIM_COUNT_CONTINUOUS);
}

