/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              本示例展示ADC单次扫描转换，配置1个通道(外部通道PB11)，使能ADC转换完成；模拟看门
*                     狗监控ADC通道11的输入电压，当输入电压超出阈值范围，LED1灯闪烁，未超出阈值LED1常灭。
*                     
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "main.h"

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
    
    /* ADC初始化 */
    adc_init();

    /* NVIC初始化 */
    nvic_init();

    /* 启动转换 */
    std_adc_start_conversion();
    
    while(1)
    {
        if(g_interrupt_status == 0x01)
        {
            /* LED1闪烁 */
            LED1_TOGGLE();
            std_delayms(300); 
        }
    }
}


