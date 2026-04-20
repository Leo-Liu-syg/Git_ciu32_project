/************************************************************************************************/
/**
* @file               usart_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              USART BSP头文件。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef USART_BSP_H
#define USART_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"

/*-------------------------------------------functions------------------------------------------*/
void bsp_usart_set_half_duplex(USART_t *usartx);
void bsp_usart1_tx_uart2_rx(void);
void bsp_uart2_tx_usart1_rx(void);

#ifdef __cplusplus
}
#endif

#endif /* USART_BSP_H */
