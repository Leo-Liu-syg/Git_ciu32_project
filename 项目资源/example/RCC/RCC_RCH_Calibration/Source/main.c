/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              该示例实现通过LXTAL校准RCH，以获取全温度范围内更高精度、更高可靠性的RCH时钟；
*                     在使用LXTAL的应用中，推荐在使用RCH时钟前（如上电或stop低功耗唤醒后）或通过RTC定时，
*                     周期性对RCH校准，校准后的RCH时钟精度可达0.4%。
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

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{
    /* 配置系统时钟为RCH（16MHz） */
    system_clock_config();
    
    /* 配置LXTAL，并使能 */
    bsp_rcc_lxtal_config(RCC_LXTAL_DRIVE_MODE_ENHANCE, RCC_LXTAL_DRIVE_LEVEL2);
    
    /* 使能MCO输出 */
    bsp_rcc_mco_config(RCC_MCO_SRC_SYSCLK, RCC_MCO_DIV1);
    
    /* TIM3初始化 */
    tim3_init();

    /* NVIC初始化 */
    nvic_init();
    
    /* RCH校准流程，校准后RCH精度可达0.4% */
    bsp_rch_calibration();    
    
    while(1)
    {

    }
}










