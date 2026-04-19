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

/*-------------------------------------------functions------------------------------------------*/
/**
* @brief  配置RCHSYS作为系统时钟，即RCH 2分频
* @retval 无
*/
void system_clock_config(void)
{
    /* 配置RCH分频为2分频 */
    std_rcc_set_rchdiv(RCC_RCH_DIV2);
    
    /* 更新系统时钟全局变量 */
    SystemCoreClockUpdate();
}

/**
* @brief  GPIO初始化配置(LED和LPTIM1 GPIO引脚初始化)
* @retval 无
*/
void gpio_init(void)
{
    std_gpio_init_t led_gpio_config = {0};
    std_gpio_init_t lptim_gpio_config = {0};
    
    /* 使能LED、LPTIM1 GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOB);
    
    /* 初始化LED1 GPIO */
    led_gpio_config.pin = LED1_PIN;
    led_gpio_config.mode = GPIO_MODE_OUTPUT;
    led_gpio_config.pull = GPIO_PULLUP;
    led_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    std_gpio_init(LED_GPIO_PORT, &led_gpio_config);    

    /* 关闭LED */   
    LED1_OFF();

    /* 初始化LPTIM1_ETR GPIO PB6 */
    lptim_gpio_config.pin = GPIO_PIN_6;
    lptim_gpio_config.mode = GPIO_MODE_ALTERNATE;
    lptim_gpio_config.pull = GPIO_PULLDOWN;
    lptim_gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    lptim_gpio_config.alternate = GPIO_AF5_LPTIM1;
    std_gpio_init(GPIOB, &lptim_gpio_config);
}

/**
* @brief  设置LPTIM1超时事件触发EXTI唤醒
* @retval 无
*/
void exti_init(void)
{
    std_exti_init_t exti_cfg = {0};
    
    exti_cfg.line_id = EXTI_LINE_LPTIM1;
    exti_cfg.mode = EXTI_MODE_EVENT;
    std_exti_init(&exti_cfg);
}

/**
* @brief  LPTIM1初始化配置
* @note   Vcore_aon域写使能开启后未关闭，用户可根据需求进行修改
* @retval 无
*/
void lptim1_init(void)
{
    std_lptim_init_t lptim_cfg = {0}; 
    
    /* 使能PMU时钟，开启VCORE_AON写使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_PMU);
    std_pmu_vaon_write_enable();
    
    /* 使能LXTAL时钟 */
    std_rcc_lxtal_drive_mode_config(RCC_LXTAL_DRIVE_MODE_ENHANCE);
    std_rcc_lxtal_drive_config(RCC_LXTAL_DRIVE_LEVEL2);
    std_rcc_lxtal_enable(RCC_LXTAL_ON);
    while(!std_rcc_get_lxtal_ready());
    
    /* 选择LXTAL作为LPTIM时钟源 */
    std_rcc_set_lptim1clk_source(RCC_LPTIM1_ASYNC_CLK_SRC_LXTAL);
    
    /* LPTIM1外设时钟使能 */
    std_rcc_apb1_clk_enable(RCC_PERIPH_CLK_LPTIM1);
    
    /* 初始化LPTIM1 */
    lptim_cfg.clock_source = LPTIM_CLK_SRC_INT;
    lptim_cfg.prescaler = LPTIM_PRESCALER_DIV1;
    lptim_cfg.count_source = LPTIM_COUNTER_CLK_INT;
    lptim_cfg.out_polarity = LPTIM_OUTPUT_POLARITY_REGULAR;
    std_lptim_init(LPTIM1, &lptim_cfg);
    
    /* 设置LPTIM1外部触发 */
    std_lptim_set_external_trig_source(LPTIM1, LPTIM_TRIG_SRC_GPIO);
    std_lptim_set_external_trig_polarity(LPTIM1, LPTIM_TRIG_EDGE_RISING);
    std_lptim_set_external_trig_filter(LPTIM1, LPTIM_TRIG_SAMPLETIME_NOFLT);
}



