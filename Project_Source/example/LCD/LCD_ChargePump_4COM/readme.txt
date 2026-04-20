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
   -- 驱动模式:   电荷泵模式
   -- 帧频率：    32Hz
   -- 屏幕类型：  1/4 Duty、1/3 Bias  VLCD = 3.3V

本示例用于演示LCD 电荷泵模式的配置过程
1、上电配置系统时钟（RCH 8MHz作为系统时钟源）；
2、配置LCD外设GPIO；
3、LCD时钟选择LXTAL；
4、LCD配置完成后在屏幕上显示数字；
5、延时3s后进入Stop模式；
6、按下按键后，从Stop模式唤醒，闪烁LED1。

