/************************************************************************************************/
/**
* @file               lcd_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              LCD BSP头文件。
*                     
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef LCD_BSP_H
#define LCD_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
#include "common.h"
    
/*------------------------------------------functions-------------------------------------------*/
void EXTI4_15_IRQHandler(void);   
#ifdef __cplusplus
}
#endif

#endif /* LCD_BSP_H */

