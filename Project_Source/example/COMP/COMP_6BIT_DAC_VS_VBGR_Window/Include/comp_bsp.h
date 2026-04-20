/************************************************************************************************/
/**
* @file               comp_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              COMP BSP头文件。
*                     
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef COMP_BSP_H
#define COMP_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
/*-------------------------------------------define---------------------------------------------*/
#define STATE_OVER_THRESHOLD    (0x00000001U)
#define STATE_WITHIN_THRESHOLD  (0x00000002U)
#define STATE_UNDER_THRESHOLD   (0x00000003U)
/*------------------------------------------functions-------------------------------------------*/
uint32_t bsp_input_voltage_level_check(void);

#ifdef __cplusplus
}
#endif

#endif /* COMP_BSP_H */

