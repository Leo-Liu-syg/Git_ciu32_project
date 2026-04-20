/************************************************************************************************/
/**
* @file               i2c_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              I2C BSP头文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef I2C_BSP_H
#define I2C_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"


/*-------------------------------------------functions------------------------------------------*/
void bsp_i2c1_master_send(void);
void bsp_i2c1_master_receive(void);

#ifdef __cplusplus
}
#endif

#endif /* I2C_BSP_H */

