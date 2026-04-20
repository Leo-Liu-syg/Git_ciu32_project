/************************************************************************************************/
/**
* @file               lcd_segment_yr1213c.c
* @author             MCU Ecosystem Development Team
* @brief              YR1213C LCD 段码屏驱动文件。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "lcd_segment_yr1213c.h"

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
COM                          0   1   2   3
SEG(2 * n)                 {   , E,  F,  A  }
SEG(2 * n + 1)             { D , C , G , B  }

n is the position of the character on the LCD screen, n = 0, 1, 2, 3.


The character '2' for example is:
                    ---------------------------------
SEG(2 * n)                 { 0,  1,  0,  1  }
SEG(2 * n + 1)             { 1,  0,  1,  1  }
                    ---------------------------------
                      '2' =  2   1   2   3 
*/

/* 数字字符表 '0' --> '9' */
const uint16_t digit_map[10] =
{
    /* 0       1         2        3         4        5        6        7        8        9  */
    0x2313,  0x0202,  0x2123,  0x2223,   0x0232,  0x2231,  0x2331,  0x0203,  0x2333,  0x2233
};

/* 数字字符像素表 */
uint32_t pixel_digit_table[4];

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  LCD屏数字字符转换
* @param  pixel 字符串指针
* @note   根据LCD的真值表转换成对应的数字字符表，再转换成字符
* @retval 无
*/
static void convert(uint8_t* pixel)
{
    uint16_t ch = 0;
    uint8_t loop = 0, index = 0;

    if((*pixel <= '9') && ( *pixel >= '0'))
    {
        ch = digit_map[*pixel - '0'];
    }  
    /* pixel_digit_table[0] 表示LCD_RAM0 
       pixel_digit_table[1] 表示LCD_RAM1 
       pixel_digit_table[2] 表示LCD_RAM2 
       pixel_digit_table[3] 表示LCD_RAM3 
    */
    for (loop = 12, index=0; index < 4; loop -= 4, index++)
    {
        pixel_digit_table[index] = (ch >> loop) & 0x0f;
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
* @retval 无
*/
void bsp_lcd_display_char(uint8_t* pixel, digit_position_t position)
{
    uint32_t number[4];
    uint32_t position_mask_list[4] = {LCD_DIGIT_POSITION1_MASK,
                                      LCD_DIGIT_POSITION2_MASK,
                                      LCD_DIGIT_POSITION3_MASK,
                                      LCD_DIGIT_POSITION4_MASK};

    convert(pixel);

    number[0] = ((pixel_digit_table[0] & 0x1) << ((position-1)*2)) | (((pixel_digit_table[0] & 0x2) >> 1) << ((position-1)*2 + 1));
    number[1] = ((pixel_digit_table[1] & 0x1) << ((position-1)*2)) | (((pixel_digit_table[1] & 0x2) >> 1) << ((position-1)*2 + 1));             
    number[2] = ((pixel_digit_table[2] & 0x1) << ((position-1)*2)) | (((pixel_digit_table[2] & 0x2) >> 1) << ((position-1)*2 + 1));
    number[3] = ((pixel_digit_table[3] & 0x1) << ((position-1)*2)) | (((pixel_digit_table[3] & 0x2) >> 1) << ((position-1)*2 + 1));
 
    std_lcd_write(LCD_DIGIT_RAM0, position_mask_list[position-1], number[0]);
    std_lcd_write(LCD_DIGIT_RAM1, position_mask_list[position-1], number[1]);
    std_lcd_write(LCD_DIGIT_RAM2, position_mask_list[position-1], number[2]);
    std_lcd_write(LCD_DIGIT_RAM3, position_mask_list[position-1], number[3]);
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
