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

/*------------------------------------------define----------------------------------------------*/
#define EEPROM_ADDRESS      (0xA2)
#define MEM_ADDRESS_SIZE    (2U)
#define MEM_ADDRESS         (0x0000)
#define DATA_LENGTH         (8U)
    
/*-------------------------------------------functions------------------------------------------*/
void bsp_i2c1_eeprom_write(void);
void bsp_i2c1_eeprom_read(void);


#ifdef __cplusplus
}
#endif

#endif /* I2C_BSP_H */

