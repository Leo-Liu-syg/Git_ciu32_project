/************************************************************************************************/
/**
* @file               wwdg_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              WWDG BSP驱动函数，计算重载后进入窗口的时间。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "wwdg_bsp.h"

/*-------------------------------------------functions------------------------------------------*/

/**
* @brief  计算WWDG计时器重载后到进入窗口内所需的时间
* @param  time_value 计时的周期数
* @note   时间计算：
          1.WWDG的时钟计数周期(ms) = (4096 * 2^7) / (PCLK1 / 1000) 
                                   = 10.922 ms
          2.WWDG的超时时间(ms)     = (0x3F + 1) * 10.922
                                  ~= 699 ms
          3.进入WWDG窗口的时间(ms)
            Window timeout(ms)     = (32 - 10 + 1) * 10.922
                                   = 251.206 ms 
* @retval uint32_t WWDG重载后到进入窗口内的所需时间
*/
uint32_t bsp_timeout_calculation(uint32_t time_value)
{
    uint32_t timeout_value = 0;
    uint32_t pclk1 = 0;
    uint32_t wdgpb = 0;

    /* 获得APB1的时钟频率 */
    pclk1 = std_rcc_get_pclk1freq();

    wdgpb = (1 << (WWDG_PRESCALER_128 >> WWDG_CFG_WDGPR_POS));

    timeout_value = ((4096 * wdgpb * time_value) / (pclk1 / 1000));

    return timeout_value;
}
