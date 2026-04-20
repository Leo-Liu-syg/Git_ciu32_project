本示例使用到的资源：
TIM5 CH1     --   PA10

本示例的功能流程：
1、上电配置时钟（PLL 48MHz）；
2、配置TIM3为时钟模式2，ETR源为LXTAL；TIM3计时时钟选择LXTAL，能够保证PWM的频率的准确性；
2、TIM5输出PWM，TIM3输入捕获PWM，并对TIM5的ARR值校准、拟合；
4、PWM输出频率范围为：”1Hz~5KHz”，输出误差最大0.05%。