/************************************************************************************************/
/**
* @file               tim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              TIM BSP驱动函数,实现TIM中断方式计数
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

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  TIM8中断服务程序
* @retval 无
*/
void TIM8_IRQHandler(void)
{
    /* TIM8更新中断处理流程 */
    if ((std_tim_get_interrupt_enable(TIM8, TIM_INTERRUPT_UPDATE)) && (std_tim_get_flag(TIM8, TIM_FLAG_UPDATE)))
    {
        std_tim_clear_flag(TIM8, TIM_FLAG_UPDATE);
        
        /* TIM8处理定时中断 */
    }
}

/**
* @brief  TIM8启动计数，并使能更新事件中断
* @retval 无
*/
void bsp_tim8_start(void)
{
    /* 使能更新中断 */
    std_tim_interrupt_enable(TIM8, TIM_INTERRUPT_UPDATE);
    
    /* 开启定时器计数 */
    std_tim_enable(TIM8);
}

