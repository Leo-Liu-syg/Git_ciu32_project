/************************************************************************************************/
/**
* @file               tamp_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              TAMP BSP驱动函数，实现TAMP功能配置。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "tamp_bsp.h"

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief TAMP中断服务函数
* @retval 无
*/
void RTC_TAMP_IRQHandler(void)
{
    /* TAMP中断处理流程 */
    if(std_tamp_get_interrupt_enable(TAMP_INTERRUPT_TAMP_IN) 
    && std_tamp_get_interrupt_flag(TAMP_INTERRUPT_FLAG_TAMP_IN))
    {
        /* 清除外部入侵检测（外部引脚）的标志 */
        std_tamp_clear_flag(TAMP_CLEAR_FLAG_TAMP_IN);
        
        g_tampin_flag = 1;
    }
}

/**
* @brief  外部引脚入侵检测配置
* @retval 无
*/
void bsp_tamp_tampin_config(void)
{
    /* 禁止外部入侵检测 */
    std_tamp_disable(TAMP_SOURCE_TAMP_IN);
    
    /* 设置入侵检测触发方式 */
    std_tamp_set_trigger(TAMP_TRIGGER_FALLING_EDGE);
        
    /* 设置入侵检测引脚数字滤波参数 */
    std_tamp_set_filter(TAMP_FILTER_DISABLE);
        
    /* 使能入侵检测引脚上拉电阻 */
    std_tamp_pullup_enable();
    
    /* 使能备份寄存器擦除 */
    std_tamp_bkp_erase_enable();
        
    /* 备份寄存器写入特征值 */
    TAMP_BACKUP_DR0 = 0x5a5a5a5a;
    TAMP_BACKUP_DR1 = 0x11111111;
    TAMP_BACKUP_DR2 = 0x22222222;
    TAMP_BACKUP_DR3 = 0x33333333;
    TAMP_BACKUP_DR4 = 0x44444444;

    /* 使能外部入侵检测中断 */
    std_tamp_interrupt_enable(TAMP_INTERRUPT_TAMP_IN);
    
    /* 使能外部入侵检测 */
    std_tamp_enable(TAMP_SOURCE_TAMP_IN);
    
    /* 配置中断优先级 */
    NVIC_SetPriority(RTC_TAMP_IRQn, NVIC_PRIO_0); 
    /* 使能中断 */
    NVIC_EnableIRQ(RTC_TAMP_IRQn); 
}
