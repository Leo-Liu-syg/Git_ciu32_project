/************************************************************************************************/
/**
* @file               spi_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              SPI BSP头文件。
*                     
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef SPI_BSP_H
#define SPI_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h" 

/*--------------------------------------------variables-----------------------------------------*/	
extern __IO uint32_t g_send_complete;
extern __IO uint32_t g_recv_complete;
    
/*-------------------------------------------functions------------------------------------------*/
void DMA_Channel0_IRQHandler(void);
void DMA_Channel1_IRQHandler(void);
void bsp_spi_dma_config(void);

#ifdef __cplusplus
}
#endif

#endif /* SPI_BSP_H */
