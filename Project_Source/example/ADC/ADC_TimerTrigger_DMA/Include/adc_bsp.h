/************************************************************************************************/
/**
* @file               adc_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              ADC BSP头文件。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef ADC_BSP_H
#define ADC_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------includes-------------------------------------------*/
#include "ciu32l051_std.h"
/*-------------------------------------------variables------------------------------------------*/
extern __IO uint8_t g_adc_complete;
extern __IO uint16_t g_dma_result;
    
/*-------------------------------------------functions------------------------------------------*/
void DMA_Channel0_IRQHandler(void);
void bsp_adc_dma_config(void);


#ifdef __cplusplus
}
#endif

#endif /* ADC_BSP_H */

