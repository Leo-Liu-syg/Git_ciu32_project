本示例使用的硬件资源：
LPUART1 TX      --   PB11  
LPUART1 RX      --   PB10

LPUART1通信参数配置:
    -- 波特率:   9600
    -- 字符长度：8bits
    -- 停止位：  1bit
    -- 奇偶校验：无
    -- 硬件流控：禁止


本示例用于演示LPUART1 使用DMA通信的过程
1、上电配置系统时钟为PLL（48MHz）；
2、配置LPUART1的GPIO；
3、LPUART1使用DMA发送方式连续发送8个数据；
4、发送完成后使用DMA接收方式连续接收8个数据。
