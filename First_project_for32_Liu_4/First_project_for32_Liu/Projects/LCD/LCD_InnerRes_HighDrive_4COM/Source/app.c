#include "app.h"

// ADC通道采集电压值
__IO uint32_t Get_CO_Voltage;    // ADC通道3 PA4采集的CO电压值
__IO uint32_t Get_VBat_Voltage;  // ADC通道2 PA3采集的电源电压值
__IO uint32_t Get_Smoke_Voltage; // ADC通道1 PA2采集的烟雾电压值

// 零点校准值
__IO uint32_t Get_CO_Zero;    // CO传感器零点电压
__IO uint32_t Get_Smoke_Zero; // 烟雾传感器零点电压

// 转换后浓度值
__IO unsigned int ppm_co = 0;    // CO浓度，单位PPM
__IO unsigned int dbm_somke = 0; // 烟雾浓度，单位db/m

// CO校准参数
unsigned int adc_co_calibration_value = 0xffff; // CO校准系数（出厂校准值）
unsigned int app_ongoing_cal_value = 0;         // 校准值校验变量

unsigned char Running_LCD_counter;  // LCD显示倒计时
unsigned char Running_LCD_Flag = 1; // LCD显示标志

App_status Status = Status_IDLE; // 全局应用状态机，默认空闲状态

/*************************************************
 * 函数名：app_get_adc_data_process
 * 功能  ：获取ADC DMA采集结果，并转换为实际电压值
 **************************************************/
void app_get_adc_data_process(void)
{
    if (g_adc_complete == 0x01U) // ADC采集完成标志
    {
        g_adc_complete = 0x0U; // 清除完成标志

        // ADC原始值转换为实际电压
        Get_Smoke_Voltage = (g_dma_result[0] * VREFBUF_VOLTAGE_3000_REF) / ADC_CONVER_SCALE;
        Get_VBat_Voltage = (g_dma_result[1] * VREFBUF_VOLTAGE_3000_REF) / ADC_CONVER_SCALE;
        Get_CO_Voltage = (g_dma_result[2] * VREFBUF_VOLTAGE_3000_REF) / ADC_CONVER_SCALE;
    }
}

/*************************************************
 * 函数名：app_value_convert
 * 功能  ：电压值转换为CO浓度(PPM)、烟雾浓度(dB/m)
 **************************************************/
void app_value_convert(void)
{
    // adc_co_calibration_value = 0xffffffff; // CO校准系数

    // CO浓度计算
    if (Get_CO_Voltage > Get_CO_Zero)
    {
        ppm_co = (Get_CO_Voltage - Get_CO_Zero) * 200 / adc_co_calibration_value;

        if ((ppm_co / 10) < 15)
        {
            ppm_co = 0;
        }
        else if ((ppm_co / 10) < 100)
        {
            ppm_co = (ppm_co - 15) * 100 / 65;
        }
        else if ((ppm_co / 10) > 9999)
        {
            ppm_co = 9999; // 最大显示限制
        }
    }
    else
    {
        ppm_co = 0;
    }

    // 烟雾浓度计算
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
        dbm_somke = 5; // 上限限制
    }
    dbm_somke *= 7;
}

/*************************************************
 * 函数名：app_Status_Control
 * 功能  ：应用状态机控制（ idle → 自检 → 校准 → 运行 → 故障 ）
 **************************************************/
void app_Status_Control(void)
{
    if (tim8_wait) // 等待计时未结束，直接返回
    {
        return;
    }

    // 状态机切换逻辑
    switch (Status)
    {
    case Status_IDLE:
        // 进入自检状态
        Status = Status_SELF_CHECK;
        tim8_wait = 1000; // 延时1000ms
        break;

    case Status_SELF_CHECK:
        // 简单自检：电压在合理范围
        if (Get_CO_Voltage > 100 && Get_CO_Voltage < 900 && Get_Smoke_Voltage > 100)
        {
            Get_CO_Zero = Get_CO_Voltage;       // 记录CO零点
            Get_Smoke_Zero = Get_Smoke_Voltage; // 记录烟雾零点
            tim8_wait = 1500;                   // 这段时间要打入高电压
            Status = Status_CAL;                // 进入校准状态
        }
        break;

    case Status_CAL:
        if (Get_CO_Voltage - Get_CO_Zero < 186)
        {
            adc_co_calibration_value = 186;
        }
        else
        {
            adc_co_calibration_value = Get_CO_Voltage - Get_CO_Zero;
        }

        Flash_Write_CO_Voltage(); // 将校准值写入Flash
        Status = Status_RUNNING;  // 进入运行状态
        break;

    case Status_RUNNING:
        adc_co_calibration_value = Flash_Get_Value(Col_Address);
        Get_CO_Zero = Flash_Get_Value(Vol_Zero_Address);
        // 校验Flash中的校准值是否正确
        if ((adc_co_calibration_value == Flash_Get_Value(Col_Address)) && Get_CO_Zero == Flash_Get_Value(Vol_Zero_Address))
        {
        }
        else
        {
            Status = Status_ERROR; // 校验失败进入故障状态
        }

        // 浓度到了某个值就触发报警
        if (Get_CO_Voltage >= (adc_co_calibration_value + Get_CO_Zero))
        {
            Status = Status_ERROR;
        }

        // 电压低于某个值就进入低电压状态
        if (Get_VBat_Voltage <= 1190)
        {
            Status = Status_Low_Power;
        }

        break;

    case Status_ERROR:
        // 故障状态如果电压下降到合理值，那么切回去正常运行状态
        if (Get_CO_Voltage <= ((90 * (adc_co_calibration_value + Get_CO_Zero) / 100)))
        {
            Status = Status_RUNNING;
        }
        break;
    case Status_Low_Power:
        // 浓度到了某个值就触发报警
        if (Get_CO_Voltage >= (adc_co_calibration_value + Get_CO_Zero))
        {
            Status = Status_ERROR;
        }
        if (Get_VBat_Voltage > 1500) // 阈值往上加一点
        {
            Status = Status_RUNNING;
        }
        break;

    default:
        break;
    }
}

/*************************************************
 * 函数名：app_LED_Control
 * 功能  ：根据系统状态控制LED指示灯
 **************************************************/
void app_LED_Control(void)
{
    switch (Status)
    {
    case Status_IDLE:
        break;

    case Status_SELF_CHECK:
        // 自检状态：红灯1秒闪烁
        if (tim8_1s_flag)
            LED_RED_HIGH();
        else
            LED_RED_LOW();
        break;

    case Status_CAL:
        LED_RED_HIGH(); // 校准中：常亮
        break;

    case Status_RUNNING:
        // 倒计时阶段：500ms闪烁
        if (tim8_500ms_flag && Running_LCD_counter > 0)
        {
            LED_RED_HIGH();
        }
        else if ((!tim8_500ms_flag) && Running_LCD_counter > 0)
        {
            LED_RED_LOW();
        }
        // 倒计时结束：1秒闪烁
        else if (Running_LCD_counter <= 0)
        {
            if (tim8_1s_flag)
                LED_RED_HIGH();
            else
                LED_RED_LOW();
        }
        break;

    case Status_ERROR:
        // 故障状态：250ms快闪
        if (tim8_250ms_flag)
            LED_RED_HIGH();
        else
            LED_RED_LOW();
        break;

    case Status_Low_Power:

        if (tim8_5s_count >= 470)
        {
            LED_RED_HIGH(); // 红灯亮
        }

        if (tim8_5s_count < 470)
        {
            LED_RED_LOW(); // 红灯灭
        }

    default:
        break;
    }
}

/*************************************************
 * 函数名：app_LCD_Control
 * 功能  ：根据状态机控制LCD显示内容
 **************************************************/
void app_LCD_Control(void)
{
    switch (Status)
    {
    case Status_IDLE:
        break;

    case Status_SELF_CHECK:
        // 自检：交替显示CO与烟雾图标，中间显示横杠
        if (tim8_1s_flag)
        {
            com4 &= ~(1 << 10);
            com5 &= ~(1 << 4);
            com5 |= 1 << 15;
            com5 |= 1 << 3;
            lcd_show_dash();
        }
        else
        {
            com5 &= ~(1 << 15);
            com5 &= ~(1 << 3);
            com4 |= 1 << 10;
            com5 |= 1 << 4;
            lcd_show_dash();
        }
        break;

    case Status_CAL:
        break;

    case Status_RUNNING:
        // 倒计时显示
        if (tim8_500ms_flag && Running_LCD_counter > 0 && Running_LCD_Flag)
        {
            Running_LCD_counter--;
            Running_LCD_Flag = 0;
            com4 &= ~(1 << 4);
            lcd_show_mid_num((unsigned int)Running_LCD_counter);
        }

        // 倒计时结束，显示浓度
        if (Running_LCD_counter <= 0)
        {
            com4 |= 1 << 4;
            app_value_convert(); // 计算浓度值

            // 1秒切换显示CO(PPM)和烟雾(dbm)
            if (tim8_1s_flag)
            {
                com4 &= ~(1 << 10);
                com5 &= ~(1 << 4);
                com5 |= 1 << 15;
                com5 |= 1 << 3;
                lcd_show_mid_num(ppm_co);
            }
            else
            {
                com5 &= ~(1 << 15);
                com5 &= ~(1 << 3);
                com4 |= 1 << 10;
                com5 |= 1 << 4;
                lcd_show_mid_num(dbm_somke);
            }
        }
        break;

    case Status_ERROR:
        // 故障：横杠与黑屏快闪
        if (tim8_250ms_flag)
            lcd_show_dash();
        else
            lcd_mid_clear();
        break;

    case Status_Low_Power:
        com4 |= 1 << 4;
        app_value_convert(); // 计算浓度值
        if (tim8_1s_flag)//低电量图标
            com5 |= ((1 << 2));
        else
            com5 &= ~((1 << 2));
        // 5秒切换显示CO(PPM)和烟雾(dbm)
        if (tim8_5s_flag)
        {
            com4 &= ~(1 << 10);
            com5 &= ~(1 << 4);
            com5 |= 1 << 15;
            com5 |= 1 << 3;
            lcd_show_mid_num(ppm_co);
        }
        else
        {
            com5 &= ~(1 << 15);
            com5 &= ~(1 << 3);
            com4 |= 1 << 10;
            com5 |= 1 << 4;
            lcd_show_mid_num(dbm_somke);
        }

    default:
        break;
    }
}