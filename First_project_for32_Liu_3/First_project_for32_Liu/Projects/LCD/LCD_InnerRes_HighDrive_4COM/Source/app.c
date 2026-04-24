#include "app.h"

__IO uint32_t Get_CO_Voltage;    // adc-??3-PA4????
__IO uint32_t Get_Smoke_Voltage; // adc-??1-PA2????
__IO uint32_t Get_CO_Zero;       // adc-??3-PA4????
__IO uint32_t Get_Smoke_Zero;    // adc-??1-PA2????

__IO unsigned int ppm_co = 0;    // CO浓度，单位PPM
__IO unsigned int dbm_somke = 0; // 烟雾浓度，单位db/m

unsigned int adc_co_calibration_value = 0xffffffff; // 假定一个校准值186，实际应用中应通过标定过程获得
unsigned int Test_co_cal_value = 0;

unsigned char Running_LCD_counter; // LCD控制
unsigned char Running_LCD_Flag = 1;

App_status Status = Status_IDLE; // 全局变量，记录当前应用状态

void app_get_adc_data_process(void)
{
    if (g_adc_complete == 0x01U)
    {
        g_adc_complete = 0x0U;
        Get_Smoke_Voltage = (g_dma_result[0] * VREFBUF_VOLTAGE_3000_REF) / ADC_CONVER_SCALE;
        Get_CO_Voltage = (g_dma_result[1] * VREFBUF_VOLTAGE_3000_REF) / ADC_CONVER_SCALE;
    }
}

void app_value_convert(void)
{
    adc_co_calibration_value = 186; // 假定一个校准值，实际应用中应通过标定过程获得
    if (Get_CO_Voltage > Get_CO_Zero)
    {
        ppm_co = (Get_CO_Voltage - Get_CO_Zero) * 200 / adc_co_calibration_value; // 计算CO浓度，假设2000PPM对应adc_co_calibration_value的电压差值
        if ((ppm_co / 10) < 35)
        {
            ppm_co = 0;
        }
        else if ((ppm_co / 10) < 100)
        {
            ppm_co = (ppm_co - 35) * 100 / 65;
        }
        else if ((ppm_co / 10) > 9999)
        {
            ppm_co = 9999;
        }
    }
    else
    {
        ppm_co = 0;
    }

    dbm_somke = ABS_DIFF_U32(Get_Smoke_Voltage, Get_Smoke_Zero) / 100;
    if (dbm_somke > 3)
    {
        dbm_somke -= 3;
    }
    else
    {
        dbm_somke = 0;
    }

    if (dbm_somke > 5)
    {
        dbm_somke = 5;
    }
    dbm_somke *= 7;
    // }
}

void app_Status_Control(void)
{
    if (tim8_wait) // 如果等待变量不为0，表示正在等待下一次TIM8更新事件的发生
    {
        return; // 跳出函数，等待下一次调用时继续处理
    }

    // 状态转换主流程
    switch (Status)
    {
    case Status_IDLE: // 空闲,检查是否有flash数据，如果有，直接使用数据并进入正常工作状态
        Status = Status_SELF_CHECK;
        tim8_wait = 1000; // 等待10秒钟，模拟自检准备过程
        break;
    case Status_SELF_CHECK:

        if (Get_CO_Voltage > 100 && Get_CO_Voltage < 900 && Get_Smoke_Voltage > 100) // 简单的自检条件
        {
            Get_CO_Zero = Get_CO_Voltage;
            Get_Smoke_Zero = Get_Smoke_Voltage;
            tim8_wait = 1000; // 等待30秒钟，模拟自检过程
            Status = Status_CAL;
            break;
        }
        break;
    case Status_CAL:
        adc_co_calibration_value = 186;
        Flash_Write_CO_Voltage(); // 写入flash
        Status = Status_RUNNING;
        break;

    case Status_RUNNING:
        Test_co_cal_value = Flash_get_Value(&adc_co_calibration_value);
#ifdef TEST_ERROR
        Test_co_cal_value = 0xffffffff;
#endif
        if (Test_co_cal_value == adc_co_calibration_value)
        {
            Status = Status_RUNNING;
        }
        else
        {
            Status = Status_ERROR;
        }
        break;
    case Status_ERROR:

        break;
    default:
        break;
    }
}

void app_LED_Control(void)
{
    // LED控制
    switch (Status)
    {
    case Status_IDLE:

        break;
    case Status_SELF_CHECK:
        if (tim8_1s_flag) // 红色LED闪烁1S亮灭
        {
            LED_RED_HIGH();
        }
        else
        {
            LED_RED_LOW();
        }
        break;
    case Status_CAL:
        LED_RED_HIGH();

        break;
    case Status_RUNNING:
        if (tim8_500ms_flag && Running_LCD_counter > 0)
        {
            LED_RED_HIGH();
            break; // 执行一次就退出
        }
        if ((!tim8_500ms_flag) && Running_LCD_counter > 0)
        {
            LED_RED_LOW();
            break;
        }
        if (Running_LCD_counter <= 0)
        {
            if (tim8_1s_flag)
            {
                LED_RED_HIGH();
            }
            else
            {
                LED_RED_LOW();
            }
        }
        break;
    case Status_ERROR:
        if (tim8_250ms_flag)
        {
            LED_RED_HIGH();
        }
        else
        {
            LED_RED_LOW();
        }
        break;
    default:
        break;
    }
}

void app_LCD_Control(void)
{

    switch (Status)
    {
    case Status_IDLE:
        break;
    case Status_SELF_CHECK:
        if (tim8_1s_flag) // ??TIM8??????
        {
            com4 &= ~(1 << 10); // smoke图标关闭
            com5 &= ~(1 << 4);  // dbm图标关闭
            com5 |= 1 << 15;    // CO图标
            com5 |= 1 << 3;     // PPM图标
            lcd_show_dash();
        }
        else
        {
            com5 &= ~(1 << 15); // CO图标关闭
            com5 &= ~(1 << 3);  // PPM图标关闭
            com4 |= 1 << 10;    // smoke图标
            com5 |= 1 << 4;     // dbm图标
            lcd_show_dash();
        }
        break;
    case Status_CAL:

        break;
    case Status_RUNNING: // 60s倒计时
        if (tim8_500ms_flag && Running_LCD_counter > 0 && Running_LCD_Flag)
        {
            Running_LCD_counter--;
            Running_LCD_Flag = 0;
            com4 &= ~(1 << 4); //| (1 << 4) 小数点关闭
            lcd_show_mid_num((unsigned int)Running_LCD_counter);
        }
        if (Running_LCD_counter <= 0)
        {
            com4 |= 1 << 4;      // 小数点
            app_value_convert(); // 先进行数值转换，更新ppm_co和dbm_somke的值
            if (tim8_1s_flag)    // ??TIM8??????
            {
                com4 &= ~(1 << 10); // smoke图标关闭
                com5 &= ~(1 << 4);  // dbm图标关闭
                com5 |= 1 << 15;    // CO图标
                com5 |= 1 << 3;     // PPM图标

                lcd_show_mid_num(ppm_co);
            }
            else
            {
                com5 &= ~(1 << 15); // CO图标关闭
                com5 &= ~(1 << 3);  // PPM图标关闭
                com4 |= 1 << 10;    // smoke图标
                com5 |= 1 << 4;     // dbm图标
                lcd_show_mid_num(dbm_somke);
            }
        }
        break;
    case Status_ERROR:
        if (tim8_250ms_flag)
        {
            lcd_show_dash();
        }
        else
        {
            lcd_mid_clear();
        }
        break;
    default:
        break;
    }
}