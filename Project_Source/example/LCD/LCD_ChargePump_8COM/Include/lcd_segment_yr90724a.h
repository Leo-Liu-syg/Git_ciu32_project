/************************************************************************************************/
/**
* @file               lcd_segment_yr90724a.h
* @author             MCU Ecosystem Development Team
* @brief              LCD YR90724A段码屏应用头文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef LCD_SEGMENT_YR90724A_H
#define LCD_SEGMENT_YR90724A_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------------------------includes--------------------------------------------*/
#include "ciu32l051_std_common.h"
    
/*-----------------------------------------type define------------------------------------------*/
/**
* @brief LCD数字显示的位置
*/
typedef enum
{
    LCD_DIGIT_POSITION_1 = 1,
    LCD_DIGIT_POSITION_2 = 2,
    LCD_DIGIT_POSITION_3 = 3,
    LCD_DIGIT_POSITION_4 = 4,
    LCD_DIGIT_POSITION_5 = 5,
    LCD_DIGIT_POSITION_6 = 6,
    LCD_DIGIT_POSITION_7 = 7,
    LCD_DIGIT_POSITION_8 = 8,
    LCD_DIGIT_POSITION_MAX = 8,
}digit_position_t;

typedef enum
{
    LCD_POINT_POSITION_1 = 1,
    LCD_POINT_POSITION_2 = 2,
    LCD_POINT_POSITION_3 = 3,
    LCD_POINT_POSITION_4 = 4,
    LCD_POINT_POSITION_5 = 5,
    LCD_POINT_POSITION_6 = 6,
    LCD_POINT_POSITION_7 = 7,
    LCD_POINT_POSITION_8 = 8,
    LCD_POINT_POSITION_MAX = 8,
}point_position_t;

typedef enum
{
    LCD_POINT_OFF  = 0,
    LCD_POINT_ON   = 1,
}point_on_t;

/*--------------------------------------------define--------------------------------------------*/
/**
* @brief LCD_RAM 编号重定义
*/
#define LCD_COM0          LCD_RAM_REGISTER0
#define LCD_COM1          LCD_RAM_REGISTER1
#define LCD_COM2          LCD_RAM_REGISTER2
#define LCD_COM3          LCD_RAM_REGISTER3
#define LCD_COM4          LCD_RAM_REGISTER4
#define LCD_COM5          LCD_RAM_REGISTER5
#define LCD_COM6          LCD_RAM_REGISTER6
#define LCD_COM7          LCD_RAM_REGISTER7

/**
* @brief LCD_RAM屏蔽位
*/
#define LCD_DIGIT_SEG_MASK   (0x00000001)
#define LCD_PONIT_SEG_MASK   (0x00000001)

/*-------------------------------------------functions------------------------------------------*/
void bsp_lcd_display_single_point(point_position_t position, point_on_t point_on);
void bsp_lcd_display_string(uint8_t* ptr);
void bsp_lcd_display_char(uint8_t* ch, digit_position_t position);


#ifdef __cplusplus
}
#endif


#endif /* LCD_SEGMENT_YR90724A_H */
