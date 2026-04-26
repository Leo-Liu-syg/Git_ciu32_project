#include "gpio.h"

/**
 * @brief  GPIO初始化函数
 * @retval 无
 */
void gpio_init(void)
{
    std_gpio_init_t gpio_config = {0};

    /* 使能所有GPIO端口时钟 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA | RCC_PERIPH_CLK_GPIOB | RCC_PERIPH_CLK_GPIOC | RCC_PERIPH_CLK_GPIOF);

    /* LCD引脚复用配置 */
    gpio_config.pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.alternate = GPIO_AF3_LCD;
    std_gpio_init(GPIOA, &gpio_config);

    gpio_config.pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    std_gpio_init(GPIOB, &gpio_config);

    gpio_config.pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    std_gpio_init(GPIOC, &gpio_config);

    /* IIC温湿度传感器GPIO配置 */
    gpio_config.pin = PIN_IIC_SCL | PIN_IIC_SDA;
    gpio_config.mode = GPIO_MODE_OUTPUT;
    gpio_config.pull = GPIO_NOPULL;
    gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    std_gpio_init(GPIO_IIC, &gpio_config);

    /* CO传感器ADC引脚配置 */
    gpio_config.mode = GPIO_MODE_ANALOG;
    /* ADC_IN2模拟输入配置 PA4 */
    gpio_config.pin = GPIO_PIN_4;
    gpio_config.mode = GPIO_MODE_ANALOG;
    std_gpio_init(GPIOA, &gpio_config);
    
    /* VREFBUF模拟输入配置 PA0*/
    gpio_config.pin = GPIO_PIN_3;
    gpio_config.mode = GPIO_MODE_ANALOG;
    std_gpio_init(GPIOA, &gpio_config);
}