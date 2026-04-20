本示例使用的硬件资源：
USART1_TX   --   PA11
USART1_RX   --   PA12
USART1_CK   --   PA10 
USART1_NSS  --   PD3

本示例可与SPI1_Slave_Interrupt示例对接测试

USART通信参数配置:
     -- 波特率:   115200
     -- 字符长度：8bits
     -- 停止位：  1bit
     -- 奇偶校验：无
     -- 通信模式：同步模式

本示例的功能流程：
1、上电配置系统时钟为PLL（48MHz）；
2、初始化GPIO、USART：
     -- 主机为USART1，从机为SPI1；
     -- 主机USART设置为大端；
     -- 使能USART同步时钟；
3、启动同步主模式发送8字节接收数据。
