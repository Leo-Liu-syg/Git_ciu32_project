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


/*------------------------------------------functions-------------------------------------------*/
void bsp_comp1_poll(void);

#ifdef __cplusplus
}
#endif

#endif /* COMP_BSP_H */

