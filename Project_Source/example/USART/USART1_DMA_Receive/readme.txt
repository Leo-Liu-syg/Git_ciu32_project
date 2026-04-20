本示例使用的硬件资源：
USART1 TX      --   PA11  
USART1 RX      --   PA12

USART通信参数配置:
     -- 波特率:   115200
     -- 字符长度：8bits
     -- 停止位：  1bit
     -- 奇偶校验：无
     -- 硬件流控：禁止


本示例的功能流程：
1、上电配置系统时钟为PLL（48MHz）；
2、初始化DMA、GPIO、USART；
3、启动DMA传输，以DMA方式接收8字节数据。
