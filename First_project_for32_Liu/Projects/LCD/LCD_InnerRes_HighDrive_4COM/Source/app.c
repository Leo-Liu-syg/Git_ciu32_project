#include "app.h"

__IO uint32_t Get_CO_Voltage;    // adc-??3-PA4????
__IO uint32_t Get_Smoke_Voltage; // adc-??1-PA2????
__IO uint32_t Get_CO_Zero;       // adc-??3-PA4????
__IO uint32_t Get_Smoke_Zero;    // adc-??1-PA2????

__IO unsigned int ppm_co = 0;    // CO浓度，单位PPM
__IO unsigned int dbm_somke = 0; // 烟雾浓度，单位db/m

unsigned char test_Hello_baby = 0;

unsigned char Running_LCD_count_60s = 60; // LCD控制
unsigned char Running_LCD_Flag=1;

typedef enum
{
    Status_IDLE = 0,   // 初始待机状态（未启动流程）
    Status_SELF_CHECK, // 传感器整体硬件自检（ADC/供电/通信）
    Status_CAL,        // 自检完成（判断是否通过）
    Status_RUNNING,    // 正常运行状态（实时采集/转换值）
    Status_ERROR       // 异常状态（自检/标定失败，需处理）
} App_status;

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
    unsigned int adc_co_calibration_value = 186; // 假定一个校准值，实际应用中应通过标定过程获得
    if (Get_CO_Voltage > Get_CO_Zero)
    {
        ppm_co = (Get_CO_Voltage - Get_CO_Zero) * 200 / adc_co_calibration_value; // 计算CO浓度，假设2000PPM对应adc_co_calibration_value的电压差值
        if (ppm_co < 35)
        {
            ppm_co = 0;
        }
        else if (ppm_co < 100)
        {
            ppm_co = (ppm_co - 35) * 100 / 65;
        }
        else if (ppm_co > 9999)
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

App_status Status = Status_IDLE; // 全局变量，记录当前应用状态
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
        tim8_wait = 2000; // 等待10秒钟，模拟自检准备过程
        break;

        /*1. LCD全显一次、语音播放一次报警语音“呜呜”、红色LED闪烁1S亮灭,LCD显示’-----’，持续3分钟
          2.1 3分钟后CO电压范围在0.1V-0.9V之间为正常，自检成功LED常亮可以进入标定状态，保存当前CO电压为V0CO
          2.2 3分钟后CO电压范围不在0.1V-0.9V之间为异常，LED持续闪烁
          3. 工人注入200PPM CO气体，当CO电压升高0.15V时进入标定状态*/
    case Status_SELF_CHECK:
        if (Get_CO_Voltage > 100 && Get_CO_Voltage < 900 && Get_Smoke_Voltage > 100) // 简单的自检条件
        {
            Get_CO_Zero = Get_CO_Voltage;
            Get_Smoke_Zero = Get_Smoke_Voltage;
            tim8_wait = 3000; // 等待30秒钟，模拟自检过程
            Status = Status_CAL;
        }
        break;

        /*4. 标定状态下红色LED熄灭，背光500ms亮灭灯闪烁，180S后标定完成，保存当前CO电压为V1CO，保存V1CO-V0CO做报警使用（需要满足CO电压升高0.15V以上，否则标定失败，黄灯常亮）
          5.标定成功后，红灯和黄灯交替闪烁1S亮灭一次，进入正常监控状态*/
    case Status_CAL:
        Status = Status_RUNNING;

        /*正常上电（已标定，flash里面有自检标定数据）：
    1. 上电播放“发0x18播放语音：CO+GAS+somke开机语音”
    2. LCD全显一秒钟、语音播放一次报警语音“呜呜”。之后三个LED闪烁1S亮灭,LCD显示’倒计时秒数’，持续3分钟
    3. 倒计时结束的时候，读当前CO电压为V0CO，当前smoke电压差值为V0SMOKE*/
    case Status_RUNNING:

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
        LED_RED_LOW();
        break;
    case Status_RUNNING:
        if (tim8_1s_flag) // 红色LED闪烁1S亮灭
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
        lcd_show_mid_num(1234);
        lcd_show_temperature(0);
        lcd_show_humidity(0);
        break;
    case Status_SELF_CHECK:
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
        lcd_show_temperature(40);
        lcd_show_humidity(30);
        break;
    case Status_CAL:
        Status = Status_RUNNING;
        break;
    case Status_RUNNING: // 60s倒计时
        if (tim8_500ms_flag && Running_LCD_count_60s > 0 && Running_LCD_Flag)
        {
            Running_LCD_count_60s--;
            Running_LCD_Flag = 0;
            lcd_show_mid_num((unsigned int)Running_LCD_count_60s);
        }
        if (Running_LCD_count_60s <= 0)
        {
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

    default:
        break;
    }
}