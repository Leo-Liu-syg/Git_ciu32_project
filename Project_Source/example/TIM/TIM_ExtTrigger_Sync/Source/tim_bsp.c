/************************************************************************************************/
/**
* @file               tim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              TIM BSP驱动函数，实现TIM异步触发同步输出功能配置。
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
* @brief  tim3输入通道使能
* @retval 无
*/
void bsp_tim3_intput_start(void)
{
    /* 使能输入捕获通道 */
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_2);;
}


/**
* @brief  tim3启动，并使能输出
* @retval 无
*/
void bsp_tim3_output_start(void)
{
    /* 使能输出通道 */
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_1);
    
    /* 使能计数 */
    std_tim_enable(TIM3);    
}


/**
* @brief  tim5启动，并使能输出
* @retval 无
*/
void bsp_tim5_output_start(void)
{
    /* 使能输出通道 */
    std_tim_ccx_channel_enable(TIM5, TIM_CHANNEL_1);
    
    /* 使能计数 */
    std_tim_enable(TIM5);    
}

