本示例使用的硬件资源：

COM0     --   PA10  
COM1     --   PA11  
COM2     --   PA12   
COM3     --   PC9

SEG0     --   PA15  
SEG1     --   PC10  
SEG2     --   PC11  
SEG3     --   PB3  
SEG4     --   PB4  
SEG5     --   PB5  
SEG6     --   PB6  
SEG7     --   PB7  

按键：   --   PC13


LCD参数配置:
   -- 驱动模式:   片内电阻分压模式、高驱
   -- 帧频率：    32Hz
   -- 屏幕类型：  1/4 Duty、1/3 Bias  工作电压 3.3V

本示例用于演示LCD 片内电阻分压、高驱的配置过程
1、上电配置系统时钟（RCH 2分频作为系统时钟，8MHz）；
2、配置LCD外设GPIO；
3、LCD时钟选择LXTAL；
4、LCD配置完成后在屏幕上显示数字；
5、按下按键，LCD显示关闭后进入Stop模式；
6、再按一下按键从Stop模式退出，LCD显示数字；
7、后续每按两次按键触发一次系统进入Stop模式，进入Stop前关闭LCD显示。
