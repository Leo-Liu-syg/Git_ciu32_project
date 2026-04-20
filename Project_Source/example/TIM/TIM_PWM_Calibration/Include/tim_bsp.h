/************************************************************************************************/
/**
* @file               tim_bsp.h
* @author             MCU Ecosystem Development Team
* @brief              TIM BSP头文件。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/


/* 避免头文件重复引用 */
#ifndef TIM_BSP_H
#define TIM_BSP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std.h"
#include "common.h"
 

/*-----------------------------------------type define------------------------------------------*/    
typedef struct 
{
    uint8_t  ch1_cc_end;
    uint32_t ch1_cc_sub_value_avg;
    uint32_t ch1_cc_sub_value_min;
    uint32_t ch1_cc_sub_value_max;
} bsp_tim3_capture_info_t;

    
/*--------------------------------------------variables-----------------------------------------*/
extern __IO bsp_tim3_capture_info_t  g_tim3_capture_info;    
    

/*-------------------------------------------functions------------------------------------------*/
void bsp_tim_psc_config(uint16_t freq);
bool bsp_check_pwm_freq(uint16_t freq);

void bsp_cal_match_freq(uint32_t obj_freq, uint32_t *match_freq);
void bsp_pwm_matching(uint32_t freq);
void bsp_pwm_calibration(uint32_t freq);
    
void bsp_tim3_capture_start(void);
void bsp_tim3_capture_stop(void);
void bsp_tim5_output_start(void);    
void bsp_tim5_output_stop(void);


#ifdef __cplusplus
}
#endif

#endif /* TIM_BSP_H */

