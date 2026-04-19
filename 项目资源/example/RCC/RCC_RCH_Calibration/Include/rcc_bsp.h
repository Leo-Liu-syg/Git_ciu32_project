/************************************************************************************************/
/**
* @file               rcc_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              RCC BSP头文件。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/


/*避免头文件重复引用*/
#ifndef RCC_BSP_H
#define RCC_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
    
    
/*-------------------------------------------functions------------------------------------------*/
void bsp_rch_calibration(void);
void bsp_tim3_capture_start(void);
void bsp_tim3_capture_stop(void);
    
void bsp_rcc_lxtal_config(uint32_t lxtal_drv_mode, uint32_t lxtal_drv_level);
void bsp_rcc_mco_config(uint32_t mco_source, uint32_t mco_psc);

void TIM3_IRQHandler(void);
    


#ifdef __cplusplus
}
#endif

#endif /* RCC_BSP_H */

