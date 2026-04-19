/************************************************************************************************/
/**
* @file               lcd_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              LCD BSP驱动函数。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "lcd_bsp.h"

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  EXTI4_15中断服务函数
* @retval 无
*/
void EXTI4_15_IRQHandler(void)
{
    if (std_exti_get_pending_status(EXTI_LINE_GPIO_PIN13))
    {
        /* 将挂起标志清零 */
        std_exti_clear_pending(EXTI_LINE_GPIO_PIN13);    
    }
}

