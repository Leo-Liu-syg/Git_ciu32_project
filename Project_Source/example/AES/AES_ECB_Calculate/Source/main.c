/************************************************************************************************/
/**
* @file               main.c
* @author             MCU Ecosystem Development Team
* @brief              应用程序主流程。
*                     
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/


/*------------------------------------------includes--------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>

/*--------------------------------------------define--------------------------------------------*/
#define BUFFER_SIZE     (8)

/*--------------------------------------------variables-----------------------------------------*/
/* 
   密钥数据 
   字节存储顺序：0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F
*/
uint32_t g_key[4] = 
{
    0x33323130, 0x37363534,
    0x3B3A3938, 0x3F3E3D3C
};

/* 
   明文数据 
   字节存储顺序：0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F
*/
uint32_t g_plain_buffer[BUFFER_SIZE] =
{
    0x33323130, 0x37363534,
    0x3B3A3938, 0x3F3E3D3C,
    0x33323130, 0x37363534,
    0x3B3A3938, 0x3F3E3D3C 
};

/* 
   预期的计算结果 
   字节存储顺序：0xFA,0xF7,0x14,0xB8,0xF9,0xCD,0x44,0x80,0xFE,0xA8,0x15,0x23,0x7B,0x73,0x48,0x0C
*/
uint32_t g_expected_value[BUFFER_SIZE] = 
{
    0xB814F7FA, 0x8044CDF9,
    0x2315A8FE, 0x0C48737B, 
    0xB814F7FA, 0x8044CDF9,
    0x2315A8FE, 0x0C48737B
};

/* 加解密运算输出数据 */
uint32_t g_result[BUFFER_SIZE] = {0};

/*-------------------------------------------functions------------------------------------------*/
int main(void)
{      
    /* 配置系统时钟 */
    system_clock_config();

    /* 加密初始化 */
    aes_encrypt_init();
    
    /* 加密明文数据 */
    if (bsp_aes_ecb_crypt(g_key, g_plain_buffer, g_result, BUFFER_SIZE*4))
    {
        std_aes_deinit();
        while(1);
    }
    /* 比较加密结果与期望数据是否一致 */
    if (memcmp(g_result, g_expected_value, BUFFER_SIZE*4))
    {
        std_aes_deinit();
        while(1);
    }
           
    /* 解密初始化 */
    aes_decrypt_init();
    
    /* 解密密文数据 */
    if (bsp_aes_ecb_crypt(g_key, g_expected_value, g_result, BUFFER_SIZE*4))
    {
        std_aes_deinit();
        while(1);
    }
    /* 比较解密结果与明文数据是否一致 */
    if (memcmp(g_result, g_plain_buffer, BUFFER_SIZE*4))
    {
        std_aes_deinit();
        while(1);
    }
    
    while(1)
    {
       
    }
}




