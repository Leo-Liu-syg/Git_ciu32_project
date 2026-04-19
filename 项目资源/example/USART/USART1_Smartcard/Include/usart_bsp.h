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
#include "common.h"
/*-------------------------------------------functions------------------------------------------*/

void EXTI4_15_IRQHandler(void);
std_status_t bsp_smartcard_send_byte(uint8_t data_buffer);
void bsp_smartcard_start_timeout_count(uint32_t cmd_timeout);
std_status_t bsp_smartcard_receive_byte(uint8_t *data_buffer);
std_status_t bsp_smartcard_cold_reset(uint8_t *data_buffer);
std_status_t bsp_smartcard_transmission(void);
void bsp_smartcard_deactivation(void);
void bsp_smartcard_activation(void);

#ifdef __cplusplus
}
#endif

#endif /* USART_BSP_H */
