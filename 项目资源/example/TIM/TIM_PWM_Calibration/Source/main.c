/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              该示例实现PWM输出频率校准功能；
*                     其中LXTAL作为TIM3的校准时钟，通过TIM3/TIM5级联方式，实现TIM5 PWM输出频率校准功能；
*                     输出范围为1Hz~5KHz，精度可达0.05%。
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

/*--------------------------------------------define--------------------------------------------*/                  
/* PWM输出的目标频率，用户可根据需求调整该定义 */
#define OBJ_FREQ_HZ                     (3000)


/*-------------------------------------------functions------------------------------------------*/
int main(void)
{    
    /* 配置系统时钟 */
    system_clock_config();
       
    /* TIM3、TIM5初始化 */
    tim3_init();
    tim5_init();
    
    /* NVIC初始化 */
    nvic_init();

    /* 根据PWM输出的目标频率，调整TIM3和TIM5的预分频 */
    bsp_tim_psc_config(OBJ_FREQ_HZ);
    
    /* 检查PWM目标频率是否是精准频率，即1，2，4，8...512，1024，2048，4096 */
    if(bsp_check_pwm_freq(OBJ_FREQ_HZ) == true)
    {
        /* PWM 校准 */
        bsp_pwm_calibration(OBJ_FREQ_HZ);
    }
    else
    {
        /* PWM 拟合 */
        bsp_pwm_matching(OBJ_FREQ_HZ);
    }

    /* 校准完成，TIM5 GPIO配置 */
    gpio_init();
    
    /* TIM5输出PWM波形 */
    bsp_tim5_output_start();
   
    while(1)
    {
    }
}

