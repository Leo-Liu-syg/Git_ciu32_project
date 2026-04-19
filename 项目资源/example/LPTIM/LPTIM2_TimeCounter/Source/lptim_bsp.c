/************************************************************************************************/
/**
* @file               lptim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              LPTIM BSP驱动函数，实现LPTIM2计数功能。
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
#define PERIODVALUE     (uint32_t) (0x8000 - 1)       /* 定时周期(ARR) */

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  LPTIM2中断服务程序
* @retval 无
*/
void LPTIM2_IRQHandler(void)
{
    if((std_lptim_get_interrupt_status(LPTIM2, LPTIM_INTERRUPT_ARRM)) && (std_lptim_get_flag(LPTIM2, LPTIM_FLAG_ARRM)))
    {
        std_lptim_clear_flag(LPTIM2, LPTIM_CLEAR_ARRM);
        
        /* LPTIM2处理重载匹配中断 */

    }
}

/**
* @brief  LPTIM2启动计数，并使能自动重载匹配中断
* @retval 无
*/
void bsp_lptim_start(void)
{
    /* 使能自动重载匹配中断 */
    std_lptim_interrupt_enable(LPTIM2, LPTIM_INTERRUPT_ARRM);
    
    /* 使能LPTIM2 */
    std_lptim_enable(LPTIM2);
    
    /* 启动LPTIM2计数 */
    std_lptim_set_auto_reload(LPTIM2, PERIODVALUE);
    std_lptim_start_counter(LPTIM2, LPTIM_COUNT_CONTINUOUS);
}



