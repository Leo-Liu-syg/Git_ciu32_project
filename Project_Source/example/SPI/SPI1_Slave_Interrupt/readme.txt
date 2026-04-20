本示例使用到的资源：
SPI1_SCK           --    PB3
SPI1_NSS           --    PD2
SPI1_MOSI          --    PB5
SPI1_MISO          --    PB4


本示例的功能流程：
1、上电配置系统时钟为PLL（48MHz）；
2、SPI1初始化为从机模式，通信模式为模式2，数据位为MSB模式；
3、SPI1作为从机以中断方式发送接收数据。