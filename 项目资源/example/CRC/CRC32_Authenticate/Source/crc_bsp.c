/************************************************************************************************/
/**
* @file               crc_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              CRC BSP驱动函数实现。
*                           
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "crc_bsp.h"

/*-------------------------------------------functions------------------------------------------*/

/**
* @brief  按字节校验CRC值
* @param  data      待计算CRC值的输入数据 
* @param  len       输入数据长度
* @param  crc_value CRC校验值
* @retval std_status_t  返回执行结果
*/
std_status_t bsp_crc32_authenticate(uint8_t *data, uint32_t len, uint32_t crc_value)
{
    uint32_t i;
    
    for (i=0; i<len; i++)
    {
        std_crc_set_byte(*(data + i));
    }
    
    std_crc_set_byte(crc_value);
    std_crc_set_byte(crc_value>>8);
    std_crc_set_byte(crc_value>>16);
    std_crc_set_byte(crc_value>>24);
    
    
    if (std_crc_get_check_flag())
    {        
        return STD_OK;
    }
    else
    {
        return STD_ERR;
    }
}
