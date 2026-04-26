#include "i2c.h"

uint8_t I2C_count = 0; // 温度计时补偿
uint32_t temperature_Raw;
uint16_t temperature_low; // 负温度
uint32_t humidity;
uint16_t temperature_F = 1; // 华式摄氏度
uint16_t temperature_S = 1; // 摄氏度
void SDA_OUT(void)
{
    std_gpio_init_t gpio_config = {0};
    gpio_config.pin = PIN_IIC_SDA;
    gpio_config.mode = GPIO_MODE_OUTPUT;
    gpio_config.output_type = GPIO_OUTPUT_PUSHPULL;
    std_gpio_init(GPIO_IIC, &gpio_config);
}

void SDA_IN(void)
{
    std_gpio_init_t gpio_config = {0};
    gpio_config.pin = PIN_IIC_SDA;
    gpio_config.mode = GPIO_MODE_INPUT;
    std_gpio_init(GPIO_IIC, &gpio_config);
}

void IIC_Start(void)
{
    SDA_OUT();
    GPIO_IIC_SDA_HIGH();
    GPIO_IIC_SCL_HIGH();
    std_delayus(10);
    GPIO_IIC_SDA_LOW();
    std_delayus(10);
    GPIO_IIC_SCL_LOW();
    std_delayus(10);
}

void IIC_Stop(void)
{
    SDA_OUT();
    GPIO_IIC_SCL_LOW();
    GPIO_IIC_SDA_LOW();
    std_delayus(10);
    GPIO_IIC_SCL_HIGH();
    std_delayus(10);
    GPIO_IIC_SDA_HIGH();
    std_delayus(10);
}

void IIC_Wait_Ack(void)
{
    SDA_IN();
    GPIO_IIC_SDA_HIGH();
    std_delayus(5);
    GPIO_IIC_SCL_HIGH();
    std_delayus(5);
    // if(Pin_SDA == 1)
    // {
    //     F_I2C_error = 1;
    // }
    GPIO_IIC_SCL_LOW();
}

void IIC_Ack(void)
{
    GPIO_IIC_SCL_LOW();
    SDA_OUT();
    GPIO_IIC_SDA_LOW();
    std_delayus(5);
    GPIO_IIC_SCL_HIGH();
    std_delayus(5);
    GPIO_IIC_SCL_LOW();
}

void IIC_NAck(void)
{
    GPIO_IIC_SCL_LOW();
    SDA_OUT();
    GPIO_IIC_SDA_HIGH();
    std_delayus(5);
    GPIO_IIC_SCL_HIGH();
    std_delayus(5);
    GPIO_IIC_SCL_LOW();
}

void IIC_Send_Byte(unsigned char txd)
{
    unsigned char t;
    SDA_OUT();
    GPIO_IIC_SCL_LOW();
    for (t = 0; t < 8; t++)
    {
        if ((txd & 0x80) >> 7)
            GPIO_IIC_SDA_HIGH();
        else
            GPIO_IIC_SDA_LOW();
        txd <<= 1;
        std_delayus(5);
        GPIO_IIC_SCL_HIGH();
        std_delayus(5);
        GPIO_IIC_SCL_LOW();
        std_delayus(5);
    }
}

uint16_t IIC_Read_Byte(void)
{
    unsigned char i;
    uint16_t receive = 0;
    SDA_IN();
    for (i = 0; i < 8; i++)
    {
        GPIO_IIC_SCL_LOW();
        std_delayus(10);
        GPIO_IIC_SCL_HIGH();
        receive <<= 1;
        if (GPIO_GET_IIC_SDA())
            receive++;
        std_delayus(10);
    }
    return receive;
}

void GXHTC3_read(void)
{
    IIC_Start();
    IIC_Send_Byte(0xE0);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x35);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x17);
    IIC_Wait_Ack();
    IIC_Stop();
    std_delayms(2);
    IIC_Start();
    IIC_Send_Byte(0xE0);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x78);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x66);
    IIC_Wait_Ack();
    IIC_Stop();
    std_delayms(12);
    IIC_Start();
    IIC_Send_Byte(0xE1);
    IIC_Wait_Ack();
    temperature_Raw = (IIC_Read_Byte() << 8);
    IIC_Ack();
    temperature_Raw |= IIC_Read_Byte();
    IIC_Ack();
    IIC_Read_Byte();
    IIC_Ack();
    humidity = (IIC_Read_Byte() << 8);
    IIC_Ack();
    humidity |= IIC_Read_Byte();
    IIC_NAck();
    IIC_Stop();

    temperature_F = (temperature_Raw * 175) / 65536;
    temperature_S = (temperature_F - 32) * 5 / 9;
    humidity = humidity * 100 / 65536;

    if (humidity > 99)
    {
        humidity = 99;
    }

    // printf("[I] temp:%d hum:%d\r\n", temperature, humidity);
}

void GXHTC3_sleep()
{
    IIC_Start();
    IIC_Send_Byte(0xE0);
    IIC_Wait_Ack();
    IIC_Send_Byte(0xB0);
    IIC_Wait_Ack();
    IIC_Send_Byte(0x98);
    IIC_Wait_Ack();
    IIC_Stop();
    std_delayms(3);
}
