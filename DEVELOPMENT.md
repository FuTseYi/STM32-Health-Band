# Xieyi智能手环开发文档

## 目录
1. [开发环境搭建](#开发环境搭建)
2. [硬件连接说明](#硬件连接说明)
3. [软件架构设计](#软件架构设计)
4. [核心模块说明](#核心模块说明)
5. [编程规范](#编程规范)
6. [调试指南](#调试指南)
7. [问题排查](#问题排查)

## 开发环境搭建

### 必需软件
1. **Keil uVision5** (推荐版本 5.29 或更高)
   - 下载地址: https://www.keil.com/
   - 需要安装STM32F1系列器件包

2. **STM32 ST-LINK Utility** (用于程序下载)
   - 或使用J-Link等其他调试器

3. **串口调试助手**
   - 推荐: 串口猎人、XCOM等

### 硬件工具
- ST-Link V2调试器（或兼容产品）
- USB转TTL串口模块（调试用）
- 万用表（硬件调试）
- 示波器（可选，信号分析）

## 硬件连接说明

### STM32F103C8T6引脚分配

#### MAX30102心率血氧传感器
```
STM32F103C8T6    MAX30102
PB8       ->     SCL (IIC时钟)
PB9       ->     SDA (IIC数据)
PB5       ->     INT (中断输入)
3.3V      ->     VIN
GND       ->     GND
```

#### ADXL345加速度传感器
```
STM32F103C8T6    ADXL345
PA4       ->     SCL (IIC时钟)
PA5       ->     SDA (IIC数据)
3.3V      ->     VCC
GND       ->     GND
```

#### OLED显示屏 (SSD1306)
```
STM32F103C8T6    OLED
PB6       ->     SCL (IIC时钟)
PB7       ->     SDA (IIC数据)
3.3V      ->     VCC
GND       ->     GND
```

#### ESP8266-01S WiFi模块
```
STM32F103C8T6    ESP8266-01S
PA9 (TX1)  ->    RX
PA10 (RX1) ->    TX
3.3V       ->    VCC
GND        ->    GND
```

#### 其他外设
```
蜂鸣器: PC13
LED指示灯: PC13 (可选)
```

## 软件架构设计

### 系统层次结构
```
应用层 (USER)
├── main.c                 # 主程序
├── 业务逻辑处理
└── 用户接口

硬件抽象层 (HARDWAR)
├── MAX30102.c/h          # 心率血氧传感器
├── adxl345.c/h           # 加速度传感器
├── OLED.c/h              # OLED显示
├── timer.c/h             # 定时器
└── IO_Init.c/h           # GPIO初始化

系统层 (SYSTEM)
├── delay.c/h             # 延时函数
├── sys.c/h               # 系统配置
└── usart.c/h             # 串口通信

底层驱动 (FWLIB)
└── STM32标准外设库

硬件层 (CORE)
└── STM32启动文件和核心文件
```

### 中断优先级配置
```c
NVIC_PriorityGroup_2     // 2位抢占优先级，2位响应优先级

MAX30102中断:    抢占优先级2, 响应优先级2
定时器中断:      抢占优先级1, 响应优先级1
串口中断:        抢占优先级0, 响应优先级0
```

## 核心模块说明

### 1. 主程序流程 (main.c)
```c
int main(void)
{
    // 系统初始化
    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init();
    
    // 外设初始化
    Beep_Init();
    usart1_Init(115200);
    OLED_Init();
    Esp8266_AP_Init();
    max30102_init();
    Init_ADXL345();
    TIM3_Int_Init(9999,719);
    
    // 主循环
    while(1) {
        blood_Loop();           // 心率血氧数据处理
        if(max30102_flag == 1) {
            max30102_flag = 0;
            OLED_show_data();   // 更新显示
        }
        
        // WiFi数据传输
        if(time_100ms > 1) {
            // 发送数据到手机APP
            send_data_to_app();
            alarm_deal();       // 处理报警
        }
    }
}
```

### 2. MAX30102心率血氧检测
```c
// 初始化函数
void max30102_init(void);

// 数据读取函数
void max30102_read_fifo(float *output_data);

// 血液数据处理
void blood_Loop(void);

// 温度读取
int max30102_read_temp(void);
```

**关键算法**:
- PPG信号滤波处理
- 峰值检测算法
- SpO2计算公式: SpO2 = 110 - 25 * R (R为红光/红外光比值)

### 3. ADXL345加速度传感器
```c
// 初始化函数
void Init_ADXL345(void);

// 读取三轴数据
void Single_Read_ADXL345(void);

// 计步算法
void step_counter_process(void);

// 跌倒检测
void fall_detection(void);
```

**计步算法**:
- 三轴加速度矢量计算
- 动态阈值调整
- 步数滤波去噪

**跌倒检测算法**:
- 总加速度计算: `Total_G = sqrt(X² + Y² + Z²)`
- 跌倒阈值: `Total_G > 3g` 或 `Total_G < 0.5g`

### 4. OLED显示控制
```c
// 初始化
void OLED_Init(void);

// 显示字符串
void OLED_ShowStr(u8 x, u8 y, u8 *str, u8 size);

// 格式化显示
void OLED_printf(u8 x, u8 y, const char *fmt, ...);

// 清屏
void OLED_Clear(void);
```

### 5. WiFi通信 (ESP8266)
```c
// AP模式初始化
void Esp8266_AP_Init(void);

// 数据发送
void send_data_to_app(void);
```

**数据格式**:
```
格式: "%dR/mW%.1f%%W%.1fCW%dW%dW/r/n"
参数: 心率, 血氧, 温度, 步数, 跌倒标志
示例: "75R/mW98.5%W36.5CW1234W0W/r/n"
```

## 编程规范

### 命名规范
- **函数名**: 小写字母+下划线 `max30102_init()`
- **变量名**: 小写字母+下划线 `blood_data`
- **宏定义**: 大写字母+下划线 `MAX30102_DEVICE_ADDRESS`
- **结构体**: 首字母大写驼峰 `BloodData`

### 注释规范
```c
/**
 * @brief  函数功能简要描述
 * @param  参数1 参数1说明
 * @param  参数2 参数2说明
 * @retval 返回值说明
 * @note   重要注意事项
 * @author Xieyi
 * @date   2025-06-25
 */
```

### 代码格式
- 缩进: 使用Tab或4个空格
- 大括号: K&R风格
- 行长度: 不超过100字符

## 调试指南

### 串口调试
1. **连接串口**
   - 波特率: 115200
   - 数据位: 8
   - 停止位: 1
   - 校验位: 无

2. **调试信息输出**
```c
printf("Heart Rate: %d BPM\r\n", heart_rate);
printf("SpO2: %.1f%%\r\n", spo2);
printf("Temperature: %.1f°C\r\n", temperature);
```

### 常见调试方法
1. **LED状态指示**
2. **串口打印关键变量**
3. **OLED显示调试信息**
4. **示波器观察信号波形**

## 问题排查

### 1. 心率检测不准确
**可能原因**:
- 传感器接触不良
- 环境光干扰
- 运动伪影

**解决方法**:
- 检查硬件连接
- 调整传感器位置
- 优化滤波算法

### 2. WiFi连接失败
**可能原因**:
- ESP8266模块供电不足
- 串口通信异常
- AT指令格式错误

**解决方法**:
- 检查3.3V供电
- 验证串口波特率
- 添加延时等待

### 3. OLED显示异常
**可能原因**:
- IIC通信错误
- 显示缓存溢出
- 字体数据错误

**解决方法**:
- 检查IIC时序
- 清空显示缓存
- 验证字体数组

### 4. 计步不准确
**可能原因**:
- 阈值设置不当
- 传感器校准偏差
- 算法参数不匹配

**解决方法**:
- 调整检测阈值
- 重新校准传感器
- 优化滤波参数

## 性能优化建议

### 1. 功耗优化
- 使用STM32低功耗模式
- 合理设置传感器采样率
- 优化WiFi传输频率

### 2. 实时性优化
- 使用DMA传输数据
- 优化中断处理时间
- 减少主循环处理负载

### 3. 精度优化
- 多次采样平均
- 动态校准算法
- 温度补偿机制

## 扩展开发

### 添加新传感器
1. 创建驱动文件 `sensor_xxx.c/h`
2. 实现初始化和数据读取函数
3. 在主程序中调用相关函数
4. 更新显示和数据传输格式

### 修改通信协议
1. 定义新的数据格式
2. 修改发送函数
3. 更新手机APP解析逻辑

---

## 技术支持

如有开发问题，请：
1. 查阅本文档和代码注释
2. 在项目仓库提交Issue
3. 参考STM32和传感器官方文档

**作者**: Xieyi  
**最后更新**: 2025-06-25
