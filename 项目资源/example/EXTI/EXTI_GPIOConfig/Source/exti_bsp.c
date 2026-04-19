/************************************************************************************************/
/**
* @file               exti_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              EXTI BSP驱动函数，实现EXTI功能配置。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "exti_bsp.h"


/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  EXTI4_15中断服务函数
* @retval 无
*/
void EXTI4_15_IRQHandler(void)
{
    /* 读取EXTI通道中断挂起状态 */
    if (std_exti_get_pending_status(EXTI_LINE_GPIO_PIN13))
    {
        /* 清除EXTI通道中断挂起状态 */
        std_exti_clear_pending(EXTI_LINE_GPIO_PIN13);
        g_exti_gpio_flag = 1;
    }
}




