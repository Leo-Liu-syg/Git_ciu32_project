/************************************************************************************************/
/**
* @file               dma_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              DMA BSP头文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef DMA_BSP_H
#define DMA_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"

/*--------------------------------------------variables-----------------------------------------*/
extern __IO uint32_t g_complete;

/*-------------------------------------------functions------------------------------------------*/
void DMA_Channel0_IRQHandler(void);
void bsp_dma_config(uint32_t *source, uint32_t *destination, uint32_t number);

#ifdef __cplusplus
}
#endif

#endif /* DMA_BSP_H */
