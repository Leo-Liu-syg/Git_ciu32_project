/************************************************************************************************/
/**
* @file               adc_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              ADC BSP驱动函数。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "adc_bsp.h"

/*------------------------------------------variables-------------------------------------------*/
__IO uint8_t g_interrupt_status = 0;

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  adc中断服务函数
* @retval 无
*/
void ADC_COMP_IRQHandler(void)
{
    if((std_adc_get_flag(ADC_FLAG_AWDG1)) && (std_adc_get_interrupt_enable(ADC_INTERRUPT_AWDG1)))
    {
        /* 清除AWDG1标志 */
        std_adc_clear_flag(ADC_FLAG_AWDG1);
        g_interrupt_status = 1;
    }
}

