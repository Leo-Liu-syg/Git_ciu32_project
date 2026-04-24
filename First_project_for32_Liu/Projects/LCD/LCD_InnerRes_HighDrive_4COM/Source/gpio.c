#include "gpio.h"

void gpio_init(void)
{
    std_gpio_init_t gpio_config = {0};

    /* GPIO时钟使能 */
    std_rcc_gpio_clk_enable(RCC_PERIPH_CLK_GPIOA | RCC_PERIPH_CLK_GPIOB | RCC_PERIPH_CLK_GPIOC | RCC_PERIPH_CLK_GPIOF);

    /* LCD */
    gpio_config.pin = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_11 | GPIO_PIN_12;
    gpio_config.mode = GPIO_MODE_ALTERNATE;
    gpio_config.alternate = GPIO_AF3_LCD;
    std_gpio_init(GPIOA, &gpio_config);

    gpio_config.pin = GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
    std_gpio_init(GPIOB, &gpio_config);

    gpio_config.pin = GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11;
    std_gpio_init(GPIOC, &gpio_config);

    /* IIC温湿度 */
    gpio_config.pin = PIN_IIC_SCL | PIN_IIC_SDA;
    gpio_config.mode = GPIO_MODE_OUTPUT;
    gpio_config.pull = GPIO_NOPULL;
    gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    std_gpio_init(GPIO_IIC, &gpio_config);

    /* ADCCO */
    gpio_config.mode = GPIO_MODE_ANALOG;
    /* ADC_IN2输入通道配置：PA4 */
    gpio_config.pin = GPIO_PIN_4;
    gpio_config.mode = GPIO_MODE_ANALOG;
    /* ADC_IN1输入通道配置：PA2 */
    gpio_config.pin = GPIO_PIN_2;
    gpio_config.mode = GPIO_MODE_ANALOG;
    std_gpio_init(GPIOA, &gpio_config);
    
    // /* VREFBUF输出引脚：PA0 */
    // adc_gpio_config.pin = GPIO_PIN_0;
    // adc_gpio_config.mode = GPIO_MODE_ANALOG;
    // std_gpio_init(GPIOA, &adc_gpio_config);
}