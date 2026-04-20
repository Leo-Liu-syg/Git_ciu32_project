/************************************************************************************************/
/**
* @file               rcc_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              RCC BSP驱动函数，实现RCC时钟校准功能。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "rcc_bsp.h"


/*--------------------------------------------define--------------------------------------------*/
/* RCH校准精度范围 */
#define RCH_ACCURACY           (2)
#define RCH_COUNTER_MAX        (RCH_VALUE/LXTAL_VALUE + RCH_ACCURACY)
#define RCH_COUNTER_MIN        (RCH_VALUE/LXTAL_VALUE - RCH_ACCURACY)

#define CCX_SAMPLE_NUM         (10)

/*--------------------------------------------variables-----------------------------------------*/
uint32_t g_ch4_counter = 0;
uint32_t g_ch4_ccx_value_first = 0;
uint32_t g_ch4_ccx_value_last = 0;

__IO uint32_t g_ch4_ccx_end = 0;
__IO uint32_t g_ccx_sub_value_avg = 0;

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  TIM3中断处理程序
* @retval 无
*/
void TIM3_IRQHandler(void)
{
    uint32_t ccx_sub_value_temp;
    
    /* CC4中断处理流程 */
    if ((std_tim_get_flag(TIM3, TIM_FLAG_CC4)) && (std_tim_get_interrupt_enable(TIM3, TIM_INTERRUPT_CC4)))
    {
        std_tim_clear_flag(TIM3, TIM_FLAG_CC4);
        g_ch4_counter++;
                
        /* 获取首次捕获值 */        
        if(g_ch4_counter == 1)
        {        
            g_ch4_ccx_value_first = std_tim_get_ccx_value(TIM3, TIM_CHANNEL_4);        
        }
        
        if(g_ch4_counter == CCX_SAMPLE_NUM)
        {
            /* 获取最后一次捕获值 */
            g_ch4_ccx_value_last = std_tim_get_ccx_value(TIM3, TIM_CHANNEL_4);

            /* 关闭CC4中断 */
            std_tim_interrupt_disable(TIM3, TIM_INTERRUPT_CC4);
            
            ccx_sub_value_temp = 0;
            g_ch4_counter = 0;
            
            /* 计算捕获均值 */            
            ccx_sub_value_temp = g_ch4_ccx_value_last - g_ch4_ccx_value_first;               
            g_ccx_sub_value_avg = ccx_sub_value_temp/(CCX_SAMPLE_NUM - 1);                
               
            g_ch4_ccx_end = 1;
        }
    }    
}


/**
* @brief  使用二分法，校准RCH频率
* @retval 无
*/
void bsp_rch_calibration(void)
{ 
    uint32_t trimming_value;
    uint32_t adjust_value;
    
    /* 初始化校准参数、调整参数 */
    trimming_value = 64U;
    adjust_value = 32U;
    
    while(adjust_value != 0U)
    {
        /* 向RCC_RCHCAL校准寄存器写入校准参数 */
        std_rcc_write_rch_calibration(trimming_value);
        
        /* TIM3启动计数，并使能捕获事件中断 */
        bsp_tim3_capture_start();
        
        /* 等待输入捕获完成 */
        while(!g_ch4_ccx_end);
            
        /* TIM3停止计数，并禁止捕获事件中断 */
        bsp_tim3_capture_stop();
            
        /* 通过LXTAL输入捕获的cc值，校准RCH时钟 */
        if(g_ccx_sub_value_avg > RCH_COUNTER_MAX)
        {
            /* 本次校准参数 减去调整参数 */
            trimming_value -= adjust_value;
        }
        else if(g_ccx_sub_value_avg < RCH_COUNTER_MIN)
        {
            /* 本次校准参数加上调整参数 */
            trimming_value += adjust_value;
        }
        else
        {
             break;
        }
        
        /* 调整参数右移，得到新的调整参数 */
        adjust_value >>= 1U;
    }
}


/**
* @brief  TIM3启动计数，并使能捕获事件中断
* @retval 无
*/
void bsp_tim3_capture_start(void)
{
    g_ch4_ccx_end = 0;
    
    /* 清除通道4输入捕获标志 */
    std_tim_clear_flag(TIM3, TIM_FLAG_CC4);
    
    /* 使能TIM3输入捕获通道4中断 */
    std_tim_interrupt_enable(TIM3, TIM_INTERRUPT_CC4);
    
    /* 使能输入捕获 */
    std_tim_ccx_channel_enable(TIM3, TIM_CHANNEL_4);
    
    /* 启动TIM3计数 */
    std_tim_enable(TIM3);
}


/**
* @brief  TIM3停止计数，并禁止捕获事件中断
* @retval 无
*/
void bsp_tim3_capture_stop(void)
{
    /* 禁止输入捕获 */
    std_tim_ccx_channel_disable(TIM3, TIM_CHANNEL_4);
    
    /* 停止TIM3计数 */
    std_tim_disable(TIM3);
    
    /* TIM计数值清0 */
    std_tim_set_counter(TIM3, 0x00);
}



/**
* @brief  配置LXTAL时钟参数，包含模式及驱动能力
* @param  lxtal_drv_mode LXTAL驱动模式选择
*             @arg RCC_LXTAL_DRIVE_MODE_NORMAL： 普通模式
*             @arg RCC_LXTAL_DRIVE_MODE_ENHANCE：增强模式
* @param  lxtal_drv_level LXTAL驱动能力配置
*             @arg RCC_LXTAL_DRIVE_LEVEL0
*             @arg RCC_LXTAL_DRIVE_LEVEL1
*             @arg RCC_LXTAL_DRIVE_LEVEL2
*             @arg RCC_LXTAL_DRIVE_LEVEL3
* @note   当LXTAL已使能，则不能配置LXTAL时钟相关参数，需将该时钟禁止后，再进行配置。
* @note   配置LXTAL时，需要解除备份域的写保护，故调用该函数后，会使能PMU时钟，同时使能备份域的写保护。
* @retval 无
*/
void bsp_rcc_lxtal_config(uint32_t lxtal_drv_mode, uint32_t lxtal_drv_level)
{    
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    
    /* 要更新LXTAL配置，必须解除备份域的写保护 */
    std_pmu_vaon_write_enable();
    
    /* 配置LXTAL驱动模式参数 */
    std_rcc_lxtal_drive_mode_config(lxtal_drv_mode);
    
    /* 配置LXTAL驱动能力 */
    std_rcc_lxtal_drive_config(lxtal_drv_level);
    
    /* 使能LXTAL */
    std_rcc_lxtal_enable(RCC_LXTAL_ON);
    
    /* 等待LXTAL时钟稳定 */
    while(!std_rcc_get_lxtal_ready());
}


/**
* @brief  配置MCO输出使能
* @param  mco_source MCO输出时钟源
*             @arg  RCC_MCO_SRC_DISABLE：禁止MCO
*             @arg  RCC_MCO_SRC_SYSCLK： MCO的时钟源为系统时钟
*             @arg  RCC_MCO_SRC_RCH：    MCO的时钟源为RCH
*             @arg  RCC_MCO_SRC_HXTAL：  MCO的时钟源为HXTAL
*             @arg  RCC_MCO_SRC_PLLCLK： MCO的时钟源为PLL
*             @arg  RCC_MCO_SRC_RCL：    MCO的时钟源为RCL
*             @arg  RCC_MCO_SRC_LXTAL：  MCO的时钟源为LXTAL
* @param  mco_psc MCO输出时钟分频值
*             @arg  RCC_MCO_DIV1：  1分频
*             @arg  RCC_MCO_DIV2：  2分频
*             @arg  RCC_MCO_DIV4：  4分频
*             @arg  RCC_MCO_DIV8：  8分频
*             @arg  RCC_MCO_DIV16： 16分频
*             @arg  RCC_MCO_DIV32： 32分频
*             @arg  RCC_MCO_DIV64： 64分频
*             @arg  RCC_MCO_DIV128：128分频
* @retval 无
*/
void bsp_rcc_mco_config(uint32_t mco_source, uint32_t mco_psc)
{
    std_gpio_init_t mco_gpio_cfg = {0};
    
    /* 配置PA8作为MCO输出引脚 */    
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA);
    
    mco_gpio_cfg.pin = GPIO_PIN_8;
    mco_gpio_cfg.mode = GPIO_MODE_ALTERNATE;
    mco_gpio_cfg.alternate = GPIO_AF0_MCO;
    mco_gpio_cfg.pull = GPIO_NOPULL;
    
    std_gpio_init(GPIOA, &mco_gpio_cfg);
    
    std_rcc_mco_config(mco_source, mco_psc);
}

