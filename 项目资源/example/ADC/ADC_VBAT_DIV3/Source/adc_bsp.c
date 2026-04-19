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

/*-------------------------------------------includes-------------------------------------------*/
#include "adc_bsp.h"

/*--------------------------------------------define--------------------------------------------*/
/* ADC采样次数，根据实际需求可调整此值 */
#define ADC_SAMPLE_NUM                      (10U)

/*-------------------------------------------variables------------------------------------------*/
__IO uint8_t g_adc_complete = 0;
static uint32_t g_vbat_div3_channel_sample = 0;

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  adc中断服务函数
* @retval 无
*/
void ADC_COMP_IRQHandler(void)
{
    if((std_adc_get_interrupt_enable(ADC_INTERRUPT_EOC)) && (std_adc_get_flag(ADC_FLAG_EOC)))
    {
        /* 清除EOC中断标志 */                       
        std_adc_clear_flag(ADC_FLAG_EOC); 
        
        /* 获取VBAT/3通道采样值 */ 
        g_vbat_div3_channel_sample = std_adc_get_conversion_value();
        
        g_adc_complete = 1;
    }
}

/**
* @brief  VABT/3通道采样值转换为实际电压值
* @note   此函数先利用内部基准电压VBGR来计算ADC参考电压VDDA实际的电压，
*         然后再计算通道VBAT/3电压，单位mV
* @retval uint32_t 转换电压值
*/
uint32_t bsp_get_channel_voltage(void)
{
    uint8_t i;
    uint32_t get_vbat_div3_sample_sum = 0;
    uint32_t get_vbat_div3_sample = 0;
    uint32_t get_channel_voltage = 0;

    /* ADC多次采样取平均，提升采样精度 */
    for(i = 0; i < ADC_SAMPLE_NUM; i++)
    {
        /* 启动转换 */
        std_adc_start_conversion();
        
        /* 等待ADC通道转换完成 */
        while(g_adc_complete == 0x00);
         
        g_adc_complete = 0;

        get_vbat_div3_sample_sum += g_vbat_div3_channel_sample;
    }
    
    /* 计算平均值 */
    get_vbat_div3_sample =  get_vbat_div3_sample_sum / ADC_SAMPLE_NUM;
    
    /* 获取通道采样电压 */
    get_channel_voltage = (uint32_t)((get_vbat_div3_sample * VREFBUF_VOLTAGE_2048_REF) / ADC_CONVER_SCALE);

    /* 返回转换结果 */
    return get_channel_voltage;
}
