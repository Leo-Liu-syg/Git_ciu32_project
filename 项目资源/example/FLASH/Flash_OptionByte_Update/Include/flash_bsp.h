/************************************************************************************************/
/**
* @file               flash_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              FLASH BSP头文件
*                           
*
*************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
*************************************************************************************************/

/* 避免头文件重复引用 */
#ifndef FLASH_BSP_H
#define FLASH_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"

/*-------------------------------------------define---------------------------------------------*/
#define OB_BYTES_NUM             (6U)               /* 选项字节个数 */

/*------------------------------------------functions-------------------------------------------*/
void bsp_flash_option_byte_config(void);

#ifdef __cplusplus
}
#endif

#endif /* FLASH_BSP_H */

