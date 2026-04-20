/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              应用程序主流程。
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
    /* 配置系统时钟为PLL（48MHz），且PLL输入源为HXTAL，HXTAL频率默认为8MHz */
    bsp_rcc_pll_selhxtal();
    
    /* 配置RCH分频为2分频 */
    std_rcc_set_rchdiv(RCC_RCH_DIV2);

    /* 使能HXTAL CSS */
    std_rcc_hxtal_css_enable();
    
    /* 使能MCO输出 */
    bsp_rcc_mco_config(RCC_MCO_SRC_SYSCLK, RCC_MCO_DIV16);
    
    while(1)
    {

    }
}










