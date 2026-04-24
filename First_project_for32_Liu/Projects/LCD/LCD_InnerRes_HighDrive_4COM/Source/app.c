#include "app.h"

__IO uint32_t Get_CO_Voltage;    // adc-??3-PA4????
__IO uint32_t Get_Smoke_Voltage; // adc-??1-PA2????
__IO uint32_t Get_CO_Zero;       // adc-??3-PA4????
__IO uint32_t Get_Smoke_Zero;    // adc-??1-PA2????

__IO unsigned int ppm_co = 0;    // COŨ�ȣ���λPPM
__IO unsigned int dbm_somke = 0; // ����Ũ�ȣ���λdb/m

unsigned int adc_co_calibration_value = 0xffffffff; // �ٶ�һ��У׼ֵ186��ʵ��Ӧ����Ӧͨ���궨���̻��
unsigned int Test_co_cal_value = 0;

unsigned char Running_LCD_counter; // LCD����
unsigned char Running_LCD_Flag = 1;

App_status Status = Status_IDLE; // ȫ�ֱ�������¼��ǰӦ��״̬

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
    adc_co_calibration_value = 186; // �ٶ�һ��У׼ֵ��ʵ��Ӧ����Ӧͨ���궨���̻��
    if (Get_CO_Voltage > Get_CO_Zero)
    {
        ppm_co = (Get_CO_Voltage - Get_CO_Zero) * 200 / adc_co_calibration_value; // ����COŨ�ȣ�����2000PPM��Ӧadc_co_calibration_value�ĵ�ѹ��ֵ
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
    if (tim8_wait) // ����ȴ�������Ϊ0����ʾ���ڵȴ���һ��TIM8�����¼��ķ���
    {
        return; // �����������ȴ���һ�ε���ʱ��������
    }

    // ״̬ת��������
    switch (Status)
    {
    case Status_IDLE: // ����,����Ƿ���flash���ݣ�����У�ֱ��ʹ�����ݲ�������������״̬
        Status = Status_SELF_CHECK;
        tim8_wait = 1000; // �ȴ�10���ӣ�ģ���Լ�׼������
        break;
    case Status_SELF_CHECK:

        if (Get_CO_Voltage > 100 && Get_CO_Voltage < 900 && Get_Smoke_Voltage > 100) // �򵥵��Լ�����
        {
            Get_CO_Zero = Get_CO_Voltage;
            Get_Smoke_Zero = Get_Smoke_Voltage;
            tim8_wait = 1000; // �ȴ�30���ӣ�ģ���Լ����
            Status = Status_CAL;
            break;
        }
        break;
    case Status_CAL:
        adc_co_calibration_value = 186;
        Flash_Write_CO_Voltage(); // д��flash
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
    // LED����
    switch (Status)
    {
    case Status_IDLE:

        break;
    case Status_SELF_CHECK:
        if (tim8_1s_flag) // ��ɫLED��˸1S����
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
            break; // ִ��һ�ξ��˳�
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
            com4 &= ~(1 << 10); // smokeͼ��ر�
            com5 &= ~(1 << 4);  // dbmͼ��ر�
            com5 |= 1 << 15;    // COͼ��
            com5 |= 1 << 3;     // PPMͼ��
            lcd_show_dash();
        }
        else
        {
            com5 &= ~(1 << 15); // COͼ��ر�
            com5 &= ~(1 << 3);  // PPMͼ��ر�
            com4 |= 1 << 10;    // smokeͼ��
            com5 |= 1 << 4;     // dbmͼ��
            lcd_show_dash();
        }
        break;
    case Status_CAL:

        break;
    case Status_RUNNING: // 60s����ʱ
        if (tim8_500ms_flag && Running_LCD_counter > 0 && Running_LCD_Flag)
        {
            Running_LCD_counter--;
            Running_LCD_Flag = 0;
            com4 &= ~(1 << 4); //| (1 << 4) С����ر�
            lcd_show_mid_num((unsigned int)Running_LCD_counter);
        }
        if (Running_LCD_counter <= 0)
        {
            com4 |= 1 << 4;      // С����
            app_value_convert(); // �Ƚ�����ֵת��������ppm_co��dbm_somke��ֵ
            if (tim8_1s_flag)    // ??TIM8??????
            {
                com4 &= ~(1 << 10); // smokeͼ��ر�
                com5 &= ~(1 << 4);  // dbmͼ��ر�
                com5 |= 1 << 15;    // COͼ��
                com5 |= 1 << 3;     // PPMͼ��

                lcd_show_mid_num(ppm_co);
            }
            else
            {
                com5 &= ~(1 << 15); // COͼ��ر�
                com5 &= ~(1 << 3);  // PPMͼ��ر�
                com4 |= 1 << 10;    // smokeͼ��
                com5 |= 1 << 4;     // dbmͼ��
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