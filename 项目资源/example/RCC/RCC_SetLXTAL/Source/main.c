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
    /* 配置系统时钟为PLL（48MHz） */
    system_clock_config();

    /* 配置LXTAL，并使能 */
    bsp_rcc_lxtal_config(RCC_LXTAL_DRIVE_MODE_ENHANCE, RCC_LXTAL_DRIVE_LEVEL2);

    /* 使能MCO输出 */
    bsp_rcc_mco_config(RCC_MCO_SRC_LXTAL, RCC_MCO_DIV1);
    
    while(1)
    {

    }
}


