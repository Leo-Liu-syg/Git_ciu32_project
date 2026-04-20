/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              本示例展示TIM3的TRIG_OUT作为硬件触发源，通过DMA中断的方式获取ADC通道2的输入电
*                     压值；TIM3触发ADC转换，转换完成DMA传输，最后将采样值转换成电压值。
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/


/*------------------------------------------includes--------------------------------------------*/
#include "main.h"
/*------------------------------------------variables-------------------------------------------*/
__IO uint32_t g_get_voltage; // ADC通道2采样电压值

/*------------------------------------------functions-------------------------------------------*/
int main(void)
{
    /* 配置系统时钟 */
    system_clock_config(); 
    
    /* 延时函数初始化 */
    std_delay_init();

    /* GPIO初始化 */
    gpio_init();
    
    /* VREFBUF初始化 */
    vrefbuf_init();
    
    /* TIM3初始化 */
    timer3_init();

    /* ADC初始化 */
    adc_init();
    
    /* DMA初始化 */
    dma_init();
    
    /* DMA传输配置 */
    bsp_adc_dma_config();

    /* NVIC初始化 */
    nvic_init();
      
    /* 使能TIM3 */
    std_tim_enable(TIM3);
    std_adc_start_conversion();

    while(1)
    {
        if(g_adc_complete == 0x01U)
        {
            g_adc_complete = 0x0U;
            
            /* 获取通道采样电压,g_dma_result对应通道2电压采样值 */
            g_get_voltage = (g_dma_result * VREFBUF_VOLTAGE_3000_REF) / ADC_CONVER_SCALE;
        }
    }
}

