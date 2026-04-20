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

/*-------------------------------------------functions------------------------------------------*/

/**
* @brief  ADC通道采样值转换为实际温度值
* @note   此函数用于计算TS通道采集的温度值
* @retval uint32_t 温度值
*/
uint32_t bsp_get_ts_temperature(void)
{
    uint8_t i = 0;
    uint32_t get_single_sample_sum = 0; 
    uint32_t get_ts_sample = 0;
    uint32_t get_ts = 0;
    
     /* ADC多次采样取平均，提升采样精度 */
    for(i = 0; i < ADC_SAMPLE_NUM; i++)
    {
        /* 启动转换 */
        std_adc_start_conversion();
        
        /* 等待channel1转换完成 */
        while(std_adc_get_flag(ADC_FLAG_EOC) == 0);

        /* 清除EOC标志 */
        std_adc_clear_flag(ADC_FLAG_EOC);
        
        /* 获取外部通道采样值 */
        get_single_sample_sum += std_adc_get_conversion_value();
    }
    
    /* 计算平均值 */
    get_ts_sample =  get_single_sample_sum / ADC_SAMPLE_NUM;
    
    /* 获取温度传感器的温度值 */
    get_ts = ADC_CALC_TEMPERATURE(VREFBUF_VOLTAGE_2048_REF, get_ts_sample);
    
    /* 返回转换结果 */
    return get_ts;
}
