/************************************************************************************************/
/**
* @file               common.c
* @author             MCU Ecosystem Development Team
* @brief              通用函数或本外设相关的配置实现函数。
*
*
**************************************************************************************************
* @attention
* Copyright (c) CEC Huada Electronic Design Co.,Ltd. All rights reserved.
*
**************************************************************************************************
*/

/*------------------------------------------includes--------------------------------------------*/
#include "common.h"

/*-------------------------------------------define---------------------------------------------*/
#define SPEED_100K_TIMING       (0x10305397)
#define SLAVE_ADDRESS           (0x5C)

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  系统时钟配置
* @retval 无
*/
void system_clock_config(void)
{
    /* 设置Flash读访问等待时间 */
    std_flash_set_latency(FLASH_LATENCY_2CLK);

    /* 使能RCH */
    std_rcc_rch_enable();
    while(!std_rcc_get_rch_ready());

    /* 配置PLL相关参数，并使能 */
    std_rcc_pll_config(RCC_PLLSRC_RCH, RCC_PLLM_MUL12, RCC_PLLN_DIV2, RCC_PLL_DIV2);
    std_rcc_pll_enable();
    while(!std_rcc_get_pll_ready());
    
    std_rcc_pll_output_enable();

    /* 设置系统时钟源为PLL */
    std_rcc_set_sysclk_source(RCC_SYSCLK_SRC_PLLCLK);
    while(std_rcc_get_sysclk_source() != RCC_SYSCLK_SRC_STATUS_PLLCLK);

    /* 设置AHB分频因子 */
    std_rcc_set_ahbdiv(RCC_HCLK_DIV1);
    
    /* 设置APB1、APB2分频因子 */
    std_rcc_set_apb1div(RCC_PCLK1_DIV1);
    std_rcc_set_apb2div(RCC_PCLK2_DIV1);
    
    SystemCoreClockUpdate();
}

/**
* @brief  I2C和LED1 GPIO初始化
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t led_gpio_config = {0};
    std_gpio_init_t i2c_gpio_config = {0};
    
    /* 使能I2C LED1 GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    
    /* 初始化LED1 GPIO */
    led_gpio_config.pin = LED1_PIN;
    led_gpio_config.mode = GPIO_MODE_OUTPUT;
    led_gpio_config.pull = GPIO_PULLUP;
    led_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    std_gpio_init(LED_GPIO_PORT, &led_gpio_config);   
    
    /* 初始化关闭LED1 */
    LED1_OFF();
    
    /* 
    I2C GPIO初始化
    I2C_SCL: PB8
    I2C_SDA: PB9
    */
    i2c_gpio_config.pin = GPIO_PIN_8 | GPIO_PIN_9;
    i2c_gpio_config.mode = GPIO_MODE_ALTERNATE;
    i2c_gpio_config.pull = GPIO_PULLUP;
    i2c_gpio_config.output_type = GPIO_OUTPUT_OPENDRAIN;
    i2c_gpio_config.alternate = GPIO_AF6_I2C1;
    std_gpio_init(GPIOB, &i2c_gpio_config);
}

/**
* @brief  I2C1初始化为从机模式
* @note   从机模式时钟延长默认打开
* @retval 无
*/
void i2c1_slave_init(void)
{
    std_i2c_init_t i2c1_init_config = {0};
    
    /* 使能I2C1外设时钟 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_I2C1);

    /* 禁止I2C1 */
    std_i2c_disable(I2C1);
    
    /* I2C从模式初始化 */
    i2c1_init_config.timing = SPEED_100K_TIMING;
    i2c1_init_config.digital_filter = I2C_DIGITALFILTER_1CLK;
    i2c1_init_config.address1_length = I2C_ADDRESS1MODE_7BIT;
    i2c1_init_config.address1 = SLAVE_ADDRESS;
    i2c1_init_config.acknowledge_type = I2C_DATA_ACK;
    std_i2c_init(I2C1, &i2c1_init_config);
}

/**
* @brief  NVIC初始化
* @retval 无
*/
void nvic_init(void)
{
    NVIC_SetPriority(I2C1_IRQn, NVIC_PRIO_0);
    NVIC_EnableIRQ(I2C1_IRQn);
}
