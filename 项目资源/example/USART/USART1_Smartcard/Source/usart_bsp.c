/************************************************************************************************/
/**
* @file               usart_bsp.c
* @author             MCU Ecosystem Development Team
* @brief              USART BSP函数，实现USART配置等功能。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "usart_bsp.h"

/*------------------------------------------defines--------------------------------------------*/
#define ATR_TIMEOUT              ((uint32_t)(40000/372))                            /* 400~40000个CLK 内应接收到ATR, 本例中使用372分频 */
#define ATR_LENGTH               (12U)                                              /* 预计接收ATR信息的字符数 */
#define WT_TIMEOUT               (9600U)                                            /* T=0 中应在9600etu的时间内接收到下一帧数据 */
#define TIME_1_ETU               (((uint32_t)((1/SC_CLKOUT_FREQUECY)*372)*1000000)) /* 1etu时间，单位us */
#define ATR_TO_SEND_1ST_APDU     (16*TIME_1_ETU)                                    /* 发送APDU前的等待时间 */
#define INS_TO_NEXT_DATA         (4*TIME_1_ETU)                                     /* APDU内发送等待时间 */
#define RST_LOW_TIME             (11U)                                              /* RST信号低电平时间，单位1ms */

/*--------------------------------------------variables-----------------------------------------*/
uint8_t  g_atr_info[ATR_LENGTH] = {0};
uint8_t  g_sw_info[2] = {0};
extern __IO uint8_t g_card_inserted;

/*-------------------------------------------functions------------------------------------------*/

/**
* @brief  EXTI4_15中断服务函数
* @retval 无
*/
void EXTI4_15_IRQHandler(void)
{
    /* 读取EXTI通道中断挂起状态 */
    if (std_exti_get_pending_status(SC_DETECT_EXTI_PIN))
    {
        /* 清除EXTI通道中断挂起状态 */
        std_exti_clear_pending(SC_DETECT_EXTI_PIN);
        /* 检查插卡状态 */
        if(!std_gpio_get_input_pin(SC_GPIO_PORT, SC_GPIO_DETECT_PIN))
        {
            std_delayus(250);
            std_exti_interrupt_disable(SC_DETECT_EXTI_PIN);
            if(!std_gpio_get_input_pin(SC_GPIO_PORT, SC_GPIO_DETECT_PIN))
            {
                g_card_inserted = 1;
                std_exti_interrupt_enable(SC_DETECT_EXTI_PIN);
            }
        }
    }
}

/**
* @brief  智能卡交互流程
* @retval std_status_t 程序运行结果
*/
std_status_t bsp_smartcard_transmission(void)
{
    uint8_t send_buffer[5]={0x00,0xA4,0x00,0x00,0x02};
    uint32_t i = 0;
    std_status_t status = STD_OK;
    
    /* 获取ATR数据 */
    status = bsp_smartcard_cold_reset(g_atr_info);
    if(STD_OK != status)
    {
        return status;
    }
    
    /* 检查TS是否为 0x3b */
    if (g_atr_info[0] != 0x3b)
    {
        return STD_ERR;
    }
    /* 冷复位后延迟一段时间，再发送指令 */
    std_delayus(ATR_TO_SEND_1ST_APDU);
    /* 发送指令： 00 A4 00 00 02 */
    for(i = 0; i < 5; i++)
    {
        status = bsp_smartcard_send_byte(send_buffer[i]);
        if(STD_OK != status)
        {
            return status;
        }; 
    }
    /* 启动发送指令后超时计时 */
    bsp_smartcard_start_timeout_count(WT_TIMEOUT); 
    
    /* 卡片返回 A4 */
    status =  bsp_smartcard_receive_byte(&g_sw_info[0]);
    if (STD_OK != status)
    {
        return  status;
    }
    /* 完成一次交互后关闭接收超时 */   
    std_usart_receive_timeout_disable(USART1);
    
    /* 检查过程字节 */
    if (0xA4 == g_sw_info[0])
    {
        /* 等待相反方向连续字符间隔时间 */
        std_delayus(INS_TO_NEXT_DATA);
        /* 继续发送指令：3F 00 */
        status = bsp_smartcard_send_byte(0x3F); 
        if (STD_OK != status)
        {
            return  status;
        }
        status = bsp_smartcard_send_byte(0x00);
        if (STD_OK != status)
        {
            return  status;
        }
    }
    else
    {
        return STD_ERR;
    }

    /* 启动发送指令后超时计时 */
    bsp_smartcard_start_timeout_count(WT_TIMEOUT);
    /* 卡片返回2字节内容 */
    for(i = 0; i < 2; i++)
    {
        status = bsp_smartcard_receive_byte(&g_sw_info[i]);
        if (STD_OK != status)
        {
            return  status;
        }
    }
    /* 完成一次交互后关闭接收超时 */ 
    std_usart_receive_timeout_disable(USART1);   

    return  STD_OK;  
}

/**
* @brief  智能卡冷复位
* @param  data_buffer   ATR信息存储区
* @note   检测到插卡后，电源上电，CK引脚输出时钟，RST引脚先拉低11ms再拉高
* @note   RST拉高后接收ATR信息，并使用接收超时功能监测数据流
* @retval std_status_t 程序运行结果
*/
std_status_t bsp_smartcard_cold_reset(uint8_t *data_buffer)
{
    uint32_t i = 0;
    std_status_t status = STD_OK;

    /* 触点激活并产生复位信号 */
    bsp_smartcard_activation();
    
    /* ATR 首帧字符应在40000个clk内接收*/
    bsp_smartcard_start_timeout_count(ATR_TIMEOUT);
    status = bsp_smartcard_receive_byte(&g_atr_info[0]);
    if (STD_OK != status)
    {
        return status;
    }

    /* 配置接收字符间的超时计时,后续字符间隔不应超过9600etu */
    bsp_smartcard_start_timeout_count(WT_TIMEOUT);
    for(i = 1; i < ATR_LENGTH; i++)
    {
        status = bsp_smartcard_receive_byte(&g_atr_info[i]);
        if (STD_OK != status)
        {
            return status;
        }
    }
    /* 完成一次交互后关闭接收超时 */
    std_usart_receive_timeout_disable(USART1);
    
    return STD_OK;
}

/**
* @brief  启动超时计时，用于监控交互超时时间
* @param  cmd_timeout 超时时间
* @retval 无
*/
void bsp_smartcard_start_timeout_count(uint32_t cmd_timeout)
{
    /* 配置超时计时时间 */
    std_usart_set_receive_timeout(USART1, cmd_timeout);
    
    /* 使能超时计时 */
    std_usart_receive_timeout_enable(USART1); 
}

/**
* @brief  智能卡模式单字符接收，同时检查超时和校验错误
* @param  data_buffer 接收缓冲区
* @retval std_status_t 程序运行结果
*/
std_status_t bsp_smartcard_receive_byte(uint8_t *data_buffer)
{
    /* 等待接收非空标志RXNE置1 */
    while(!std_usart_get_flag(USART1, USART_FLAG_RXNE))
    {
        if (std_usart_get_flag(USART1, USART_FLAG_RTO))
        {
            /* 如果发生超时，清零超时标志，关闭超时功能，返回超时错误状态 */
            std_usart_clear_flag(USART1, USART_CLEAR_RTO);
            std_usart_receive_timeout_disable(USART1);
            return STD_ERR_TIMEOUT;
        }
        /* 检查校验错误 */
        if(std_usart_get_flag(USART1, USART_FLAG_PE))
        {
            /* 出现校验错误时，清零错误标志，关闭超时功能，返回错误状态 */
            std_usart_clear_flag(USART1, USART_CLEAR_PE);
            std_usart_receive_timeout_disable(USART1);
            return STD_ERR;
        }
    }
    /* 获取接收数据 */
    *data_buffer = std_usart_rx_read_data(USART1);
    return STD_OK;
}

/**
* @brief  智能卡模式单字符发送
* @param  data_buffer    待发送数据
* @retval std_status_t   程序运行结果
*/
std_status_t bsp_smartcard_send_byte(uint8_t data_buffer)
{
    /* 等待上一帧数据发送完成 */
    while(!std_usart_get_flag(USART1, USART_FLAG_TC));       
    /* 填写发送数据 */
    std_usart_tx_write_data(USART1, (uint32_t)data_buffer);
    /* 等待保护时间前发送完成标志TCBGT */
    while(!std_usart_get_flag(USART1, USART_FLAG_TCBGT));       
    /* 检查错误标志 */
    if (std_usart_get_flag(USART1, USART_FLAG_FE))
    {
        return STD_ERR;
    }
    return STD_OK;
}

/**
* @brief  智能卡触点激活，并产生复位信号
* @retval 无
*/
void bsp_smartcard_activation(void)
{
    std_gpio_init_t gpio_config = {0};
    
    /* 上电 */
    std_gpio_set_pin(SC_GPIO_POWER_CTRL_PORT,SC_GPIO_POWER_CTRL_PIN);
    
    /* 配置SCIO口使能 */
    gpio_config.pin = SC_USART_SCIO_PIN;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.output_type = GPIO_OUTPUT_OPENDRAIN;
    gpio_config.pull = GPIO_PULLUP;
    gpio_config.alternate = GPIO_AF1_USART1;
    std_gpio_init(SC_USART_PORT, &gpio_config);
    
    /* 使能时钟输出 */
    std_usart_disable(USART1);
    std_usart_clk_enable(USART1);
    
    /* USART 外设使能 */
    std_usart_enable(USART1);
    
    /* 产生复位信号 */
    std_gpio_reset_pin(SC_GPIO_PORT,SC_GPIO_RST_PIN);
    /* 复位低电平在40000~45000clk， 4MHz时钟下不低于10ms */
    std_delayms(RST_LOW_TIME);
    std_gpio_set_pin(SC_GPIO_PORT,SC_GPIO_RST_PIN);
}

/**
* @brief  智能卡触点释放
* @retval 无
*/
void bsp_smartcard_deactivation(void)
{
    std_gpio_init_t gpio_config = {0};
    /* RST引脚输出低电平 */
    std_gpio_reset_pin(SC_GPIO_PORT,SC_GPIO_RST_PIN);
    
    /* 时钟关闭 */
    std_usart_disable(USART1);
    std_usart_clk_disable(USART1);
    
    /* SCIO口输出低电平 */
    gpio_config.pin = SC_USART_SCIO_PIN;
    gpio_config.mode = GPIO_MODE_OUTPUT;
    gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    gpio_config.pull = GPIO_NOPULL;
    std_gpio_init(SC_USART_PORT, &gpio_config);
    std_gpio_reset_pin(SC_USART_PORT,SC_USART_SCIO_PIN);
     
    /* 下电 */
    std_gpio_reset_pin(SC_GPIO_POWER_CTRL_PORT,SC_GPIO_POWER_CTRL_PIN);
}
