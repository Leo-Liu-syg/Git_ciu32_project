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

/*--------------------------------------------variables-----------------------------------------*/
extern __IO uint32_t g_complete;

/*-------------------------------------------functions------------------------------------------*/
void DMA_Channel1_IRQHandler(void);
void bsp_usart_dma_config(uint8_t *source,uint32_t number);

#ifdef __cplusplus
}
#endif

#endif /* USART_BSP_H */
