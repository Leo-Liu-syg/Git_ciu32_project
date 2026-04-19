自检流程：
1. LCD全显一次、语音播放一次报警语音“呜呜”、红色LED闪烁1S亮灭,LCD显示’-----’，持续3分钟
2.1 3分钟后CO电压范围在0.1V-0.9V之间为正常，自检成功LED常亮可以进入标定状态，保存当前CO电压为V0CO
2.2 3分钟后CO电压范围不在0.1V-0.9V之间为异常，LED持续闪烁
3. 工人注入200PPM CO气体，当CO电压升高0.15V时进入标定状态
4. 标定状态下红色LED熄灭，背光500ms亮灭灯闪烁，180S后标定完成，保存当前CO电压为V1CO，保存V1CO-V0CO做报警使用（需要满足CO电压升高0.15V以上，否则标定失败，黄灯常亮）
5.标定成功后，红灯和黄灯交替闪烁1S亮灭一次，进入正常监控状态

正常上电（已标定，flash里面有自检标定数据）：
1. 上电播放“发0x18播放语音：CO+GAS+somke开机语音”
2. LCD全显一秒钟、语音播放一次报警语音“呜呜”。之后三个LED闪烁1S亮灭,LCD显示’倒计时秒数’，持续3分钟
3. 倒计时结束的时候，读当前CO电压为V0CO，当前smoke电压差值为V0SMOKE

正常监视：
// 1PPM CO计算：（V2000PPM-V0CO）/200; 
// CO 35PPM以下不显示
// dbm_somke = ABS_DIFF_U32(adc_smoke_abs, adc_smoke_zero) / 100; 
1. 实时显示气体浓度，如果没有CO和smoke气体浓度的情况下,中间显示0，并且co和smoke图标都亮起
2. 加入有气体浓度的情况下，co和smoke图标一秒切换一次显示，并且显示相应的浓度值
3. 当CO浓度超过V1CO-V0CO时，红色LED 200ms亮灭，语音播放co报警语音
4. 当smoke浓度超过7时，红色LED 200ms亮灭，语音播放smoke报警语音
5. 当smoke和co浓度同时超过时，红色LED 200ms亮灭，语音循环播放smoke和co报警语音

低压检测：
1. 正常监视状态下，当电池电压低于3.1V时，绿灯100s亮一秒，电池图标也显示一秒钟，并且闪烁的时候，若没有处于报警状态，需要播放一次低压语音


每完成一步上传一次git
开发流程：
1. 用例程先确认可以烧录
2. 用gpio例程去控制灯
3. 参考timer例程，用timer例程实现一个定时器(可能是1ms)
4. 参考lcd例程，把屏幕全点亮
5. 实现lcd接口函数
5.1 全显示函数 lcd_show_all
5.2 温度显示函数 lcd_show_temperature(uint8_t temperature, uint8_t celsius)
5.3 中间显示函数 lcd_show_middle(uint16_t num, uint8_t point, uint8_t icon)
5.4 湿度显示函数 lcd_show_humidity(uint8_t humidity)
5.5 其他功能函数
6. 实现语音播放函数 voice_play(uint8_t voice_id)
7. adc功能 参考adc例程 
7.1 初始化函数 adc_init
7.2 读取adc值函数 adc_read(uint8_t channel)
7.3 读co电压函数 adc_read_co(void)
7.4 读smoke电压函数 adc_read_smoke(void)
7.5 读电池电压函数 adc_read_battery(void)
8. 温湿度传感器功能
8.1 读取温湿度传感器数据函数 dht_read_data(void) -> 设置两个全局变量 temperature 和 humidity
8.2 读取温度接口函数 uint8_t dht_read_temperature(void) -> 返回温度值
8.3 读取湿度接口函数 uint8_t dht_read_humidity(void) -> 返回湿度值
9. 参考flash例程，实现flash功能
9.1 读取flash数据函数 flash_read_data(uint8_t *data, uint16_t address, uint16_t length)
9.2 写入flash数据函数 flash_write_data(uint8_t *data, uint16_t address, uint16_t length)
10. app.c 主程序
