/************************************************************************************************/
/**
* @file               aes_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              AES BSP驱动函数实现。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "aes_bsp.h"

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  ECB加解密计算
* @param  p_key 指向密钥的指针
* @param  p_in_data 指向加解密输入数据的指针 
* @param  p_out_data 指向加解密输出数据的指针 
* @param  len  加解密数据长度，单位：字节
* @note   len必须是16的整倍数             
* @retval std_status_t 返回函数执行结果 
*/
std_status_t bsp_aes_ecb_crypt(uint32_t *p_key, uint32_t *p_in_data, uint32_t *p_out_data, uint32_t len)
{
    uint32_t i;
    uint32_t tmp[BLOCK_DATA_WORD_WIDTH];
    
     /* 根据数据交换类型进行密钥交换，并设置密钥 */
    bsp_aes_swap_data(tmp, p_key);
    std_aes_write_key(tmp);
    
    /* 计算运算块数 */
    len = len >> BLOCK_DATA_WORD_WIDTH;
    
    for (i=0; i<len; i++)
    {
        /* 写入输入数据 */
        std_aes_write_data(p_in_data);
       
        /* 等待运算完成 */
        while (!std_aes_get_flag(AES_FLAG_COMPLETE));

        /* 读取运算结果数据 */
        std_aes_read_data(p_out_data);
    
        p_in_data += BLOCK_DATA_WORD_WIDTH;
        p_out_data += BLOCK_DATA_WORD_WIDTH;
    }
    
    return STD_OK;
}

/**
* @brief  将输入的数据根据数据交换功能进行数据交换
* @param  p_obj 指向交换输出数据的指针
* @param  p_src 指向交换输入数据的指针
* @retval 无
*/
void bsp_aes_swap_data(uint32_t *p_obj, uint32_t *p_src)
{
    uint32_t data_type;
    
    /* 获取数据交换类型进行数据交换 */
    data_type = std_aes_get_data_type();
    
    if (data_type == AES_SWAPPED_DATA_NONE)
    {
        p_obj[0] = *p_src++;
        p_obj[1] = *p_src++;
        p_obj[2] = *p_src++;
        p_obj[3] = *p_src++;
    }
    else
    {
        /* 交换数据 */
        p_obj[0] = bsp_aes_sw_swap_data(data_type, *p_src++);
        p_obj[1] = bsp_aes_sw_swap_data(data_type, *p_src++);
        p_obj[2] = bsp_aes_sw_swap_data(data_type, *p_src++);
        p_obj[3] = bsp_aes_sw_swap_data(data_type, *p_src);
    }    
}

/**
* @brief  根据数据交换类型进行数据交换
* @param  data_type 数据交换类型
*             @arg AES_SWAPPED_DATA_NONE
*             @arg AES_SWAPPED_DATA_16BIT
*             @arg AES_SWAPPED_DATA_8BIT
*             @arg AES_SWAPPED_DATA_1BIT
* @param  data 待交换的数据
* @retval uint32_t 交换后的结果数据
*/
uint32_t bsp_aes_sw_swap_data(uint32_t data_type, uint32_t data)
{
    uint32_t  exchanged_data;
    uint8_t   index_bit;
    
    /*字内 字节数据交换 */
    if (AES_SWAPPED_DATA_8BIT == data_type)
    {
          exchanged_data  = (data & 0xff000000) >> 24;
          exchanged_data |= (data & 0x00ff0000) >> 8;
          exchanged_data |= (data & 0x0000ff00) << 8;
          exchanged_data |= (data & 0x000000ff) << 24;
    }       
    /*字内 位数据交换 */
    else if (AES_SWAPPED_DATA_1BIT == data_type)
    {
        exchanged_data = 0;
        for (index_bit=0; index_bit<32; index_bit++)
        {
            if (data & (1 << index_bit))
            {
                exchanged_data |= 1 << (32 - 1 - index_bit);
            }
        }
    }
    /*字内 半字数据交换 */
    else if (AES_SWAPPED_DATA_16BIT == data_type)
    {
        exchanged_data  = (data & 0xffff0000) >> 16;
        exchanged_data |= (data & 0x0000ffff) << 16;
    }    
    /* 字内无需交换 */
    else 
    {
        exchanged_data = data;  
    }  
    
    return exchanged_data;
} 



