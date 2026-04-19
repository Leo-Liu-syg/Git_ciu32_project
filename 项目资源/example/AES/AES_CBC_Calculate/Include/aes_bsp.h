/************************************************************************************************/
/**
* @file               aes_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              AES BSP头文件。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef AES_BSP_H
#define AES_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
 
/*--------------------------------------------define--------------------------------------------*/
#define BLOCK_DATA_WORD_WIDTH           (4)                          /**< 算法块字大小 */

/*-------------------------------------------functions------------------------------------------*/
std_status_t bsp_aes_cbc_crypt(uint32_t *p_key, uint32_t *p_iv, uint32_t *p_in_data, uint32_t *p_out_data, uint32_t len);
void bsp_aes_swap_data(uint32_t *p_obj, uint32_t *p_src);
uint32_t bsp_aes_sw_swap_data(uint32_t data_type, uint32_t data);

#ifdef __cplusplus
}
#endif

#endif /* AES_BSP_H */

