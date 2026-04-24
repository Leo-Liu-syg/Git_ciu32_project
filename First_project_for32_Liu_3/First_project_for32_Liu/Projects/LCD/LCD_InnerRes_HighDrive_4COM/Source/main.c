#include "main.h"

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

    app_get_adc_data_process();
    /*Flash*/
    Flash_Read_Voltage_Check(&adc_co_calibration_value);
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
    Get_CO_Voltage = 3333;
#endif

#ifdef QUICK_RUN
    Running_LCD_counter = 20;
#else
    Running_LCD_counter = 60;
#endif
    while (1)
    {

        app_Status_Control();
        app_LED_Control();
        app_LCD_Control();
        if (tim8_500ms_flag)
        {
            app_get_adc_data_process();
            GXHTC3_read();
            lcd_show_temperature(temperature_S);
            lcd_show_humidity(humidity);
        }
    }
}