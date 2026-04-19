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
/* 快速编程一行字数 */
#define  FSTPG_WORD_COUNT                   (0x40U)

/*------------------------------------------functions-------------------------------------------*/
std_status_t bsp_flash_fast_program(uint32_t address, uint32_t *data_buf); 
std_status_t bsp_flash_fast_write(uint32_t address, uint32_t *data_buf);
    
#ifdef __cplusplus
}
#endif

#endif /* FLASH_BSP_H */

