/************************************************************************************************/
/**
* @file               lcd_segment_yr1213c.h
* @author             MCU Ecosystem Development Team
* @brief              LCD YR1213C段码屏应用头文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/* 避免头文件重复引用 */
#ifndef LCD_SEGMENT_YR1213C_H
#define LCD_SEGMENT_YR1213C_H

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
    LCD_DIGIT_POSITION_MAX = 4,
}digit_position_t;


/**
* @brief LCD '.'显示的位置
*/
typedef enum
{
    LCD_POINT_POSITION_1 = 1,
    LCD_POINT_POSITION_2 = 2,
    LCD_POINT_POSITION_3 = 3,
    LCD_POINT_POSITION_MAX = 3,
}point_position_t;


/*--------------------------------------------define--------------------------------------------*/
/**
* @brief LCD SEG和COM的重定义
*/
#define LCD_COM0          LCD_RAM_REGISTER0
#define LCD_COM1          LCD_RAM_REGISTER1
#define LCD_COM2          LCD_RAM_REGISTER2
#define LCD_COM3          LCD_RAM_REGISTER3

#define LCD_SEG0          (0x00000001 << LCD_SEG0_SHIFT)
#define LCD_SEG1          (0x00000001 << LCD_SEG1_SHIFT)
#define LCD_SEG2          (0x00000001 << LCD_SEG2_SHIFT)
#define LCD_SEG3          (0x00000001 << LCD_SEG3_SHIFT)
#define LCD_SEG4          (0x00000001 << LCD_SEG4_SHIFT)
#define LCD_SEG5          (0x00000001 << LCD_SEG5_SHIFT)
#define LCD_SEG6          (0x00000001 << LCD_SEG6_SHIFT)
#define LCD_SEG7          (0x00000001 << LCD_SEG7_SHIFT)
#define LCD_SEG8          (0x00000001 << LCD_SEG8_SHIFT)
#define LCD_SEG9          (0x00000001 << LCD_SEG9_SHIFT)
#define LCD_SEG10         (0x00000001 << LCD_SEG10_SHIFT)
#define LCD_SEG11         (0x00000001 << LCD_SEG11_SHIFT)
#define LCD_SEG12         (0x00000001 << LCD_SEG12_SHIFT)
#define LCD_SEG13         (0x00000001 << LCD_SEG13_SHIFT)
#define LCD_SEG14         (0x00000001 << LCD_SEG14_SHIFT)
#define LCD_SEG15         (0x00000001 << LCD_SEG15_SHIFT)
#define LCD_SEG16         (0x00000001 << LCD_SEG16_SHIFT)
#define LCD_SEG17         (0x00000001 << LCD_SEG17_SHIFT)
#define LCD_SEG18         (0x00000001 << LCD_SEG18_SHIFT)
#define LCD_SEG19         (0x00000001 << LCD_SEG19_SHIFT)
#define LCD_SEG20         (0x00000001 << LCD_SEG20_SHIFT)
#define LCD_SEG21         (0x00000001 << LCD_SEG21_SHIFT)
#define LCD_SEG22         (0x00000001 << LCD_SEG22_SHIFT)
#define LCD_SEG23         (0x00000001 << LCD_SEG23_SHIFT)
#define LCD_SEG24         (0x00000001 << LCD_SEG24_SHIFT)
#define LCD_SEG25         (0x00000001 << LCD_SEG25_SHIFT)
#define LCD_SEG26         (0x00000001 << LCD_SEG26_SHIFT)
#define LCD_SEG27         (0x00000001 << LCD_SEG27_SHIFT)
#define LCD_SEG28         (0x00000001 << LCD_SEG28_SHIFT)
#define LCD_SEG29         (0x00000001 << LCD_SEG29_SHIFT)
#define LCD_SEG30         (0x00000001 << LCD_SEG30_SHIFT)
#define LCD_SEG31         (0x00000001 << LCD_SEG31_SHIFT)

#define LCD_SEG0_SHIFT    0
#define LCD_SEG1_SHIFT    1
#define LCD_SEG2_SHIFT    2
#define LCD_SEG3_SHIFT    3
#define LCD_SEG4_SHIFT    4
#define LCD_SEG5_SHIFT    5
#define LCD_SEG6_SHIFT    6
#define LCD_SEG7_SHIFT    7
#define LCD_SEG8_SHIFT    8
#define LCD_SEG9_SHIFT    9
#define LCD_SEG10_SHIFT   10
#define LCD_SEG11_SHIFT   11
#define LCD_SEG12_SHIFT   12
#define LCD_SEG13_SHIFT   13
#define LCD_SEG14_SHIFT   14
#define LCD_SEG15_SHIFT   15
#define LCD_SEG16_SHIFT   16
#define LCD_SEG17_SHIFT   17
#define LCD_SEG18_SHIFT   18
#define LCD_SEG19_SHIFT   19
#define LCD_SEG20_SHIFT   20
#define LCD_SEG21_SHIFT   21
#define LCD_SEG22_SHIFT   22
#define LCD_SEG23_SHIFT   23
#define LCD_SEG24_SHIFT   24
#define LCD_SEG25_SHIFT   25
#define LCD_SEG26_SHIFT   26
#define LCD_SEG27_SHIFT   27
#define LCD_SEG28_SHIFT   28
#define LCD_SEG29_SHIFT   29
#define LCD_SEG30_SHIFT   30
#define LCD_SEG31_SHIFT   31

/**
* @brief LCD 数字输出定义
*/
#define LCD_DIGIT_RAM0                LCD_COM0
#define LCD_DIGIT_RAM1                LCD_COM1
#define LCD_DIGIT_RAM2                LCD_COM2
#define LCD_DIGIT_RAM3                LCD_COM3

#define LCD_DIGIT_POSITION1_MASK      ~(LCD_SEG0 | LCD_SEG1)
#define LCD_DIGIT_POSITION2_MASK      ~(LCD_SEG2 | LCD_SEG3)
#define LCD_DIGIT_POSITION3_MASK      ~(LCD_SEG4 | LCD_SEG5)
#define LCD_DIGIT_POSITION4_MASK      ~(LCD_SEG6 | LCD_SEG7)


/*-------------------------------------------functions------------------------------------------*/
void bsp_lcd_display_string(uint8_t* ptr);
void bsp_lcd_display_char(uint8_t* ch, digit_position_t position);


#ifdef __cplusplus
}
#endif


#endif /* LCD_SEGMENT_YR1213C_H */
