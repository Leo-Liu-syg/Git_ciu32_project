本示例使用的硬件资源：
LPUART1 TX      --   PB11  
LPUART1 RX      --   PB10

LPUART1通信参数配置:
  -- 波特率:   9600
  -- 字符长度：8bits
  -- 停止位：  1bit
  -- 奇偶校验：无
  -- 硬件流控：禁止


本示例用于演示LPUART1 选择时钟源为LXTAL时，查询方式通信：
1、上电配置系统时钟为PLL（48MHz）；
2、配置LPUART1的GPIO ；
3、初始化LPUART1，时钟源为LXTAL；
4、核心板使用查询TXE标志方式连续发送8个数据 ；
5、完成数据发送后,在while(1)中循环查询RXNE标志置1后，读取接收数据。
