/************************************************************************************************/
/**
* @file               wwdg_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              WWDG BSP头文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef WWDG_BSP_H
#define WWDG_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"


/*-------------------------------------------functions------------------------------------------*/
uint32_t bsp_timeout_calculation(uint32_t time_value);


#ifdef __cplusplus
}
#endif

#endif /* WWDG_BSP_H */

