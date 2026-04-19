/************************************************************************************************/
/**
* @file               common.c
* @author             MCU Ecosystem Development Team
* @brief              通用函数或本外设相关的配置实现函数。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "common.h"


/*--------------------------------------------define--------------------------------------------*/
#define TIM_ARR_VALUE        (0xFFFFU)


/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  系统时钟配置（RCH）
* @retval 无
*/
void system_clock_config(void)    
{
    /* 设置Flash读访问等待时间为1个HCLK */
    /* 因为RCH校准寄存器会配置最大值，故频率可能会超过16MHz */
    std_flash_set_latency(FLASH_LATENCY_1CLK);
    
    /* 更新系统时钟全局变量 */    
    SystemCoreClockUpdate();
}


/**
* @brief  TIM3初始化
* @retval 无
*/
void tim3_init(void)
{
    std_tim_basic_init_t basic_init_struct = {0};
    std_tim_input_capture_init_t input_capture_struct = {0};
    
    /* TIM3时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_TIM3);
    
    /* TIM3基本定时器配置 */
    basic_init_struct.counter_mode = TIM_COUNTER_MODE_UP;
    basic_init_struct.period = TIM_ARR_VALUE;
    basic_init_struct.clock_div = TIM_CLOCK_DTS_DIV1;
    basic_init_struct.prescaler = 0x00;
    basic_init_struct.auto_reload_preload = TIM_AUTORELOAD_PRE_DISABLE;
    std_tim_init(TIM3, &basic_init_struct);
    
    /* 配置TI4FP4映射到IC4上，上升沿有效 */
    input_capture_struct.input_capture_pol = TIM_INPUT_POL_RISING;
    input_capture_struct.input_capture_sel = TIM_INPUT_CAPTURE_SEL_DIRECTTI;
    input_capture_struct.input_capture_prescaler = TIM_INPUT_CAPTURE_PSC_DIV1;
    std_tim_input_capture_init(TIM3, &input_capture_struct, TIM_CHANNEL_4);
    
    /* 配置TIM3 TI4连接到LXTAL */
    std_tim_set_channel_remap(TIM3, TIM_TIM3_TI4_LXTAL, TIM_CHANNEL_4);    
}

/**
* @brief  NVIC初始化
* @retval 无
*/
void nvic_init(void)
{
    /* NVIC初始化 */
    NVIC_SetPriority(TIM3_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(TIM3_IRQn);
}
