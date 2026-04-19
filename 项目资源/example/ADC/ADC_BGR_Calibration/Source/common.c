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

/*------------------------------------------define----------------------------------------------*/    
/* BGR使能后，等待其稳定时间，单位:us */
#define BGR_RDY_DELAY            (30UL)

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  系统时钟配置函数
* @retval 无
*/
void system_clock_config(void)
{
    /* 使能RCH */
    std_rcc_rch_enable();
    while(!std_rcc_get_rch_ready());

    /* 设置系统时钟源为RCH */
    std_rcc_set_sysclk_source(RCC_SYSCLK_SRC_RCHSYS);
    while(std_rcc_get_sysclk_source() != RCC_SYSCLK_SRC_STATUS_RCHSYS);

    /* 设置AHB分频因子 */
    std_rcc_set_ahbdiv(RCC_HCLK_DIV1);
    
    /* 设置APB1、APB2分频因子 */
    std_rcc_set_apb1div(RCC_PCLK1_DIV1);
    std_rcc_set_apb2div(RCC_PCLK2_DIV1);

    /* 更新系统时钟全局变量 */
    SystemCoreClockUpdate();
}


/**
* @brief  ADC初始化配置函数
* @retval 无
*/
void adc_init(void)
{
    std_adc_init_t adc_config = {0};

    /* 使能ADC时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_ADC);
    
    /* ADC初始化配置 */
    adc_config.clock_src      = ADC_SYNC_CLK_PCLK_DIV1;               /* 同步时钟PCLK，不分频 */
    adc_config.conver_mode    = ADC_SINGLE_CONVER_MODE;               /* 单次转换模式 */
    adc_config.overrun_mode   = ADC_OVRN_MODE_OVERWRITTEN;            /* 数据溢出模式: 数据覆盖 */
    adc_config.trigger_edge   = ADC_TRIG_SW;                          /* 软件触发 */
    adc_config.sampt1         = ADC_SAMPTIME_119CYCLES;               /* 采样时间设置为119CLK，组SAMPT1 */
    adc_config.sampt2         = ADC_SAMPTIME_119CYCLES;               /* 采样时间设置为119CLK，组SAMPT2 */
    adc_config.scan_dir       = ADC_SCAN_DIR_FORWARD;                 /* 正向扫描 */
    std_adc_init(&adc_config);
    
    /* 使能通道VBGR */
    std_adc_internal_channel_enable(ADC_INTERNAL_CHANNEL_VBGREN);

    /* 等待内部通道的启动稳定时间 
       VBGR通道启动稳定时间                  15US
       VTS通道启动稳定时间                   18US
       VBAT_DIV3和VDDA_DIV3通道启动稳定时间  15US
       配置多个内部通道时，等待最大启动稳定时间即可
    */
    std_delayus(ADC_VBGR_CHANNEL_DELAY);

    /* 选择VBGR通道(通道13) */
    std_adc_fix_sequence_channel_enable(ADC_CHANNEL_VBGR);
    
    /* 配置通道VBGR采样时间选择组2 */
    std_adc_channel_sample_time_select(ADC_SAMPTIME_GROUP_2, ADC_CHANNEL_VBGR);
    
    /* 配置wait模式，避免数据未及时读取，转换溢出 */
    std_adc_wait_mode_enable();

    /* ADC参考电压源选择 */
    std_adc_set_reference_source(ADC_REFERENCE_VDDA);

    /* 使能ADC */
    std_adc_enable();
    
    /* 等待ADC使能状态稳定 
       当ADC_CK时钟大于等于6MHz，需等待2.5us稳定时间
       当ADC_CK时钟小于6MHz，需等待17个ADC时钟周期稳定时间  
    */
    std_delayus(ADC_EN_DELAY);

    /* 使能校准 */
    std_adc_calibration_enable();
    
    /* 等待校准完成 */
    while(!std_adc_get_flag(ADC_FLAG_EOCAL));
}

/**
* @brief  VBGR初始化
* @retval 无
*/
void vbgr_init(void)
{
    /* 使能PMU时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    
    /* 使能VBGR */
    std_pmu_bgr_enable();

    /* 等待BGR稳定 */
    std_delayus(BGR_RDY_DELAY);  
}
