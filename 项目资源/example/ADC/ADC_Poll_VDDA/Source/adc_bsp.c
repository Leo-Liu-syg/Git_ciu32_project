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

/*--------------------------------------------define--------------------------------------------*/
/* ADC参考电压为VDDA：3300mV。在实际应用中需根据真实的VDD电压值设置该值 */
#define VREF_ADC_VDDA_VOLTAGE               (3300U)

/* ADC采样次数，根据实际需求可调整此值 */
#define ADC_SAMPLE_NUM                      (10U)

/*------------------------------------------functions-------------------------------------------*/


/**
* @brief  转换ADC通道采样值为实际电压值
* @retval uint32_t 转换电压值
*/
uint32_t bsp_get_channel_voltage(void)
{
    uint8_t i = 0;
    uint32_t get_channel_voltage = 0;
    uint32_t get_single_sample_sum = 0;
    uint32_t get_single_sample = 0;
    
        /* ADC多次采样取平均，提升采样精度 */
    for(i = 0; i < ADC_SAMPLE_NUM; i++)
    {
        /* 启动转换 */
        std_adc_start_conversion();
        
        /* 等待ADC转换完成 */
        while(std_adc_get_flag(ADC_FLAG_EOC) == 0);

        /* 清除EOC标志 */
        std_adc_clear_flag(ADC_FLAG_EOC);
        
        get_single_sample_sum += std_adc_get_conversion_value();
    }
    
    /* 计算平均值 */
    get_single_sample =  get_single_sample_sum / ADC_SAMPLE_NUM;
    
    /* 获取通道采样电压 */
    get_channel_voltage = get_single_sample * VREF_ADC_VDDA_VOLTAGE / ADC_CONVER_SCALE;
    
    /* 返回转换结果 */
    return get_channel_voltage;
}
