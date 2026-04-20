/************************************************************************************************/
/**
* @file               tim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              TIM BSP驱动函数,实现TIM PWM输出计数
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
* @brief  TIM3通道1~通道4启动PWM输出 
* @retval 无
*/
void bsp_tim3_output_start(void)
{
    /* 使能PWM输出通道 */
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_1);
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_2);
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_3);
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_4);

    /* 使能计数 */
    std_tim_enable(TIM3);    

}

