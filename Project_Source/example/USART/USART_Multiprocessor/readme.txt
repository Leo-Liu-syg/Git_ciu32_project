本示例使用的硬件资源：
USART1 TX      --   PA11 连接 UART2 RX      --   PD4  
USART1 RX      --   PA12 连接 UART2 TX      --   PD3  
按键：   PC13
LED1：   PB14


USART1UART2通信参数配置:
     -- 波特率:     115200
     -- 字符长度：  8bits
     -- 停止位：    1bit
     -- 奇偶校验：  无
     -- 硬件流控：  禁止


本示例的功能流程：
1、上电配置系统时钟为PLL（48MHz）；
2、初始化USART1， UART2；
3、配置USART1为多机通信模式，使用地址匹配方式退出静默，并配置本机地址；
4、USART1 进入静默模式；
5、UART2循环向USART1 发送数据，USART1处于静默模式，不接收，LED1关闭；
6、按下按键，UART2向USART1 发送匹配地址后，USART1退出静默模式；
7、退出静默模式后USART1 可以接收到数据，LED1闪烁。