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

/* VREFBUF使能后，等待其稳定时间，用户可根据对VREFBUF精度要求，调整该时间参数，单位:us */
#define VREFBUF_RDY_DELAY        (1000UL)

/*------------------------------------------functions-------------------------------------------*/
/**
* @brief  系统时钟配置
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
    adc_config.clock_src      = ADC_SYNC_CLK_PCLK_DIV1;              /* 同步时钟PCLK，不分频 */
    adc_config.conver_mode    = ADC_SINGLE_CONVER_MODE;              /* 单次转换模式 */
    adc_config.overrun_mode   = ADC_OVRN_MODE_OVERWRITTEN;           /* 数据溢出模式: 数据覆盖 */
    adc_config.trigger_edge   = ADC_TRIG_SW;                         /* 禁止外部硬件触发：软件触发 */
    adc_config.sampt1         = ADC_SAMPTIME_119CYCLES;              /* 采样时间设置为119CLK，组SAMPT1 */
    adc_config.sampt2         = ADC_SAMPTIME_119CYCLES;              /* 采样时间设置为119CLK，组SAMPT2 */
    adc_config.scan_dir       = ADC_SCAN_DIR_FORWARD;                /* 正向扫描 */

    std_adc_init(&adc_config);

    /* VBAT/3和VDDA/3共用ADC内部通道14，避免同时使能无效，先关闭，再配置 */
    std_adc_internal_channel_disable(ADC_INTERNAL_CHANNEL_VBAT_DIV3 | ADC_INTERNAL_CHANNEL_VDDA_DIV3);

    /* 使能通道VBAT/3 */
    std_adc_internal_channel_enable(ADC_INTERNAL_CHANNEL_VBAT_DIV3);

    /* 等待内部通道的启动稳定时间 
       VBGR通道启动稳定时间                  15US
       VTS通道启动稳定时间                   18US
       VBAT_DIV3和VDDA_DIV3通道启动稳定时间  15US
       配置多个内部通道时，等待最大启动稳定时间即可
    */
    std_delayus(ADC_VBAT_DIV3_CHANNEL_DELAY);

    /* 选择通道VBAT/3 */
    std_adc_fix_sequence_channel_enable(ADC_CHANNEL_VBAT_DIV3);

    /* 配置VBAT/3采样时间选择组2 */
    std_adc_channel_sample_time_select(ADC_SAMPTIME_GROUP_2, ADC_CHANNEL_VBAT_DIV3);

    /* 配置wait模式，避免数据未及时读取，转换溢出 */
    std_adc_wait_mode_enable();

    /* ADC参考电压源选择 */
    std_adc_set_reference_source(ADC_REFERENCE_VREFBUF);

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
    
    /* 清除ADC转换状态，确保之前状态不影响转换 */
    std_adc_clear_flag(ADC_FLAG_ALL);

    /* 通道转换完成中断使能 */
    std_adc_interrupt_enable(ADC_INTERRUPT_EOC);
}

/**
* @brief  GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t adc_gpio_config = {0};

    /* 使能GPIOA时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA);
    
    /* VREFBUF输出引脚：PA0 */
    adc_gpio_config.pin = GPIO_PIN_0;
    adc_gpio_config.mode = GPIO_MODE_ANALOG;
    std_gpio_init(GPIOA, &adc_gpio_config);
}

/**
* @brief  VREFBUF初始化
* @retval 无
*/
void vrefbuf_init(void)
{
    /* 使能VREFBUF外设时钟 */
    std_rcc_apb2_clk_enable(RCC_PERIPH_CLK_SYSCFG);
    /* 使能PMU时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);

    /* 使能VBGR */
    std_pmu_bgr_enable();

    /* 等待BGR稳定 */
    std_delayus(BGR_RDY_DELAY);

    /* 禁止vrefbuf */
    std_vrefbuf_disable();
    /* 写入vrefbuf校准值 */
    std_rcc_write_vrefbuf_calibration(VREFBUF_CAL_VALUE_2048);
    /* 配置VREFBUF输出电压2.048V */
    std_vrefbuf_output_config(VREFBUF_VOLTAGE_2048);
    /* 使能vrefbuf */
    std_vrefbuf_enable();

    /* 等待VREFBUF稳定，用户可根据对VREFBUF精度要求，调整该时间参数，单位:us */
    std_delayus(VREFBUF_RDY_DELAY);
}


/**
* @brief  NVIC初始化
* @retval 无
*/
void nvic_init(void)
{
    /* ADC的NVIC中断初始化 */
    NVIC_SetPriority(ADC_COMP_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(ADC_COMP_IRQn);
}

