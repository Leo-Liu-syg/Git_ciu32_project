#include "LED.h"
/**
* @brief  LED1初始化
* @retval 无
*/
void led_init(void)
{
    std_gpio_init_t led_gpio_init = {0};

    /* 使能LED1对应的GPIO时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOC);

    /* 配置LED1的IO */
    led_gpio_init.pin = LED_RED_PIN;
    led_gpio_init.mode = GPIO_MODE_OUTPUT;
    led_gpio_init.pull = GPIO_PULLUP;
    led_gpio_init.output_type = GPIO_OUTPUT_PUSHPULL;
    
    /* 初始化GPIO */
    std_gpio_init(LED_RED_GPIO_PORT, &led_gpio_init);
}