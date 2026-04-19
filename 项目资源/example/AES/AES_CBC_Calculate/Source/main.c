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
   初始值数据
   字节存储顺序：0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
*/
uint32_t g_iv_buffer[4] =
{
    0xF3F2F1F0, 0xF7F6F5F4,
    0xFBFAF9F8, 0xFFFEFDFC
};

/* 
   明文数据 
   明文字节存储顺序：0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F
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
   结果字节存储顺序：0x40,0x8B,0x8B,0xA7,0x3C,0x4D,0x77,0x1B,0x2A,0x44,0x96,0x21,0xF0,0x78,0x1B,0x27,
                     0x6B,0x7C,0x44,0x68,0xF5,0x68,0x2A,0xBE,0x47,0x6C,0xD8,0x78,0x31,0x4B,0xFB,0xA0
*/
uint32_t g_expected_value[BUFFER_SIZE] = 
{
    0xA78B8B40, 0x1B774D3C,
    0x2196442A, 0x271B78F0, 
    0x68447C6B, 0xBE2A68F5,
    0x78D86C47, 0xA0FB4B31
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
    if (bsp_aes_cbc_crypt(g_key, g_iv_buffer, g_plain_buffer, g_result, BUFFER_SIZE*4))
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
    if (bsp_aes_cbc_crypt(g_key, g_iv_buffer, g_expected_value, g_result, BUFFER_SIZE*4))
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




