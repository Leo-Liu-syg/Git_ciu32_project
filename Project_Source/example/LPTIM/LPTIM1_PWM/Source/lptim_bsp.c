/************************************************************************************************/
/**
* @file               lptim_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              LPTIM BSP驱动函数，实现LPTIM PWM输出等功能
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
#define PERIODVALUE     (uint32_t) (1000 - 1)       /* 输出PWM周期(ARR) */
#define PULSEVALUE      (uint32_t) (500 - 1)        /* 输出PWM占空比(CMP) */

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  LPTIM启动计数并输出PWM
* @retval 无
*/
void bsp_lptim_pwm_start(void)
{
    /* 使能LPTIM1 */
    std_lptim_enable(LPTIM1);
    
    /* 设置PWM周期和占空比 */
    std_lptim_set_auto_reload(LPTIM1, PERIODVALUE);
    std_lptim_set_compare(LPTIM1, PULSEVALUE);
    
    /* 启动LPTIM1计数 */
    std_lptim_start_counter(LPTIM1, LPTIM_COUNT_CONTINUOUS);
}



