/************************************************************************************************/
/**
* @file               pmu_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              PMU BSP驱动函数，实现PMU PVD功能配置。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "pmu_bsp.h"

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  PVD中断服务函数
* @retval 无
*/
void PVD_IRQHandler(void)
{
    /* 清除中断标志 */
    std_pmu_clear_pvd_interrupt_flag(); 
    
    /* 用户根据PVD的中断事件，结合PVD当前的状态实现中断处理函数 */
    
}

/**
* @brief  PVD配置并使能
* @note   PVD使能后，延时至少40us后，PVD输出状态才有效。
*         如果配置数字滤波功能，该延时时间需要PVD数字滤波时间。
* @retval 无
*/
void bsp_pmu_pvd_config(void)
{
    std_pvd_init_t pvd_config = {0};
    
    /* 使能PMU时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    
    pvd_config.level = PMU_PVD_LEVEL_7;
    pvd_config.alarm_type = PMU_PVD_FALLING;
    pvd_config.filter_enable = PVD_FILTER_TIME_ENABLE;
    pvd_config.filter_time = PVD_FILTER_TIME_4PCLK;

    std_pmu_pvd_init(&pvd_config);
    
    std_pmu_pvd_enable();
    
    /* 等待PVD稳定 */
    std_delayus(PMU_PVD_DELAY);

    /* 清除PVD中断标志 */
    std_pmu_clear_pvd_interrupt_flag();
    
    /* 配置PVD中断使能 */
    std_pmu_pvd_interrupt_enable();
}

