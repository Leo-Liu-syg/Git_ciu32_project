/************************************************************************************************/
/**
* @file               lcd_segment_yr90724a.c
* @author             MCU Ecosystem Development Team
* @brief              YR90724A LCD 段码屏驱动文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "lcd_segment_yr90724a.h"

/*------------------------------------------variables-------------------------------------------*/
/* 数字字符重定义 */
uint8_t digit = 0; 

/**
================================================================================
                                   SEGMENT LCD MAPPING
================================================================================
LCD segment mapping:
                        -------------------------------------------
                                        ----A---- 
                                        |       |
                                        F       B   
                                        |       |   
                                        ----G----
                                        |       |   
                                        E       C   
                                        |       |   
                                        ----D----   

An LCD character coding is based on the following matrix:
COM                          7   6   5   4   3   2   1   0
SEG(n-1)                   { A,  B,  F,  G,  C,  E,  D,  Sn }

n is the position of the character on the screen, n = 1, 2, 3, 4, 5, 6, 7, 8


The character '2' for example is:
                            ---------------------------------
                           { A,  B,  F,  G,  C,  E,  D,  S2 }
MSB                          1   1   0   1   0   1   1   0       LSB
                            ---------------------------------
                                        '2' =  0xD6 
*/

/* 字符表 '0' --> '9' */
const uint16_t digit_map[10] =
{
    /* 0       1        2        3        4        5       6       7       8       9    */
     0xEE,   0x48,    0xD6,    0xDA,    0x78,    0xBA,    0xBE,  0xC8,   0xFE,   0xFA
};


/* 数字字符像素表 */
uint32_t pixel_table[8];

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  LCD屏数字字符转换
* @param  pixel 字符串指针
* @note   根据LCD的真值表转换成对应的数字、符号表，再转换成字符
* @retval 无
*/
static void convert(uint8_t* pixel)
{
    uint16_t ch = 0;
    uint8_t index = 0;

    /* 数字字符转换 '0' --> '9' */
    if((*pixel <= '9') && ( *pixel >= '0'))
    {
        ch = digit_map[*pixel - '0'];
    } 

    for (index = 1; index < 8; index++)
    {
        pixel_table[index] = ((ch >> index) & 0x01);
    }
}


/**
* @brief  LCD显示字符
* @param  pixel 字符指针        
* @param  position 字符显示位置
*            @arg LCD_DIGIT_POSITION_1
*            @arg LCD_DIGIT_POSITION_2
*            @arg LCD_DIGIT_POSITION_3
*            @arg LCD_DIGIT_POSITION_4
*            @arg LCD_DIGIT_POSITION_5
*            @arg LCD_DIGIT_POSITION_6
*            @arg LCD_DIGIT_POSITION_7
*            @arg LCD_DIGIT_POSITION_8
* @retval 无
*/
void bsp_lcd_display_char(uint8_t* pixel, digit_position_t position)
{
    convert(pixel);

    std_lcd_write(LCD_COM1, ~(LCD_DIGIT_SEG_MASK << (position - 1)),  pixel_table[1] << (position - 1));
    std_lcd_write(LCD_COM2, ~(LCD_DIGIT_SEG_MASK << (position - 1)),  pixel_table[2] << (position - 1));
    std_lcd_write(LCD_COM3, ~(LCD_DIGIT_SEG_MASK << (position - 1)),  pixel_table[3] << (position - 1));

    std_lcd_write(LCD_COM4, ~(LCD_DIGIT_SEG_MASK << (position - 1)),  pixel_table[4] << (position - 1));
    std_lcd_write(LCD_COM5, ~(LCD_DIGIT_SEG_MASK << (position - 1)),  pixel_table[5] << (position - 1));
    std_lcd_write(LCD_COM6, ~(LCD_DIGIT_SEG_MASK << (position - 1)),  pixel_table[6] << (position - 1));
    std_lcd_write(LCD_COM7, ~(LCD_DIGIT_SEG_MASK << (position - 1)),  pixel_table[7] << (position - 1));
}


/**
* @brief  LCD显示符号    
* @param  position 符号显示位置
*            @arg LCD_POINT_POSITION_1
*            @arg LCD_POINT_POSITION_2
*            @arg LCD_POINT_POSITION_3
*            @arg LCD_POINT_POSITION_4
*            @arg LCD_POINT_POSITION_5
*            @arg LCD_POINT_POSITION_6
*            @arg LCD_POINT_POSITION_7
*            @arg LCD_POINT_POSITION_8
* @param  point_on 小数点显示
*            @arg LCD_POINT_ON
*            @arg LCD_POINT_OFF
* @retval 无
*/
void bsp_lcd_display_single_point(point_position_t position, point_on_t point_on)
{
    std_lcd_write(LCD_COM0, ~(LCD_PONIT_SEG_MASK << (position - 1)), (point_on & LCD_PONIT_SEG_MASK) << (position - 1));
}


/**
* @brief  LCD显示字符串
* @param  ptr 字符串指针
* @retval 无
*/
void bsp_lcd_display_string(uint8_t* ptr)
{
    digit_position_t position = LCD_DIGIT_POSITION_1;

    while ((*ptr != 0) & (position <= LCD_DIGIT_POSITION_MAX))
    {
        bsp_lcd_display_char(ptr, position);
        ptr++;
        position++;
    }
}
