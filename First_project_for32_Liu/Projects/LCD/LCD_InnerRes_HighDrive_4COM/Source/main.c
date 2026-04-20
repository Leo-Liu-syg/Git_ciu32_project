#include "main.h"

__IO uint32_t g_get_voltage; // adc-??3-PA4????
unsigned char test_Hello_baby = 0;
int main(void)
{
    /*????*/
    system_clock_config();
    std_delay_init();
    gpio_init();

    adc_init(); // ADC?????
    vrefbuf_init();
    TIM3_init();          // ???timer3??ADC??
    dma_init();           // ???DMA??ADC????
    bsp_adc_dma_config(); // ??ADC?DMA?????
    TIM3_adc_Enable();    // ??TIM3?ADC

    /*LED*/
    led_init();

    TIM8_init();      // ?????
    bsp_tim8_start(); // ?????
    nvic_init();      // NVIC??

    /* LCD */
    lcd_clock_config();
    lcd_config_init();
    lcd_show_all();

/*ºê¿ª¹Ø²âÊÔ*/
#ifdef Temp_test
    lcd_show_temperature(25);
#endif
#ifdef Hum_test
    lcd_show_humidity(50);
#endif
#ifdef Mid_test
    lcd_show_mid_num(1234);
#endif
#ifdef CO_test
    g_get_voltage = 3333;
#endif

    while (1)
    {
        if (tim8_500ms_flag) // ??TIM8??????
        {
            tim8_500ms_flag = 0; // ????
            /* ?????TIM8?????????LED */
            LED_RED_FLIP();
        }
        if (g_adc_complete == 0x01U)
        {
            g_adc_complete = 0x0U;
            test_Hello_baby = 1;
            /* ????????,g_dma_result????2????? */
            g_get_voltage = (g_dma_result * VREFBUF_VOLTAGE_3000_REF) / ADC_CONVER_SCALE;
        }
    }
}