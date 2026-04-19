本示例使用到的资源：
无

本示例的功能流程：
1、上电配置系统时钟为PLL（48MHz）；
2、配置DMA通道0初始化；
3、启动DMA，以Burst传输类型将Flash中数据搬运至RAM区，在中断服务函数判断数据传输完成；
4、g_destination[NUMBER_TO_TRANSFER]中数据更新为g_source[NUMBER_TO_TRANSFER]中的数据。
