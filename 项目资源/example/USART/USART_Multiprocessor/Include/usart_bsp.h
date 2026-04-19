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
void EXTI4_15_IRQHandler(void);
void bsp_set_usart_multiprocessor_mode(USART_t *usartx, uint8_t addr);

#ifdef __cplusplus
}
#endif

#endif /* USART_BSP_H */
