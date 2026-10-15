# AD9833 信号发生器

基于 STM32F103 + AD9833 DDS 芯片的数字信号发生器，使用 Keil5 开发。

## 功能特性

- **三种波形输出**：正弦波 (SIN)、三角波 (TRI)、方波 (SQU)
- **频率调节**：1 Hz ~ 12 MHz，支持多级步长（1 / 10 / 100 / 1k / 10k / 100k / 1M）
- **幅值调节**：0 ~ 255 级可调（通过外部增益控制芯片）
- **旋转编码器**：顺时针增加，逆时针减少，外部中断触发
- **按键切换模式**：循环切换 4 种调节模式（波形 / 步长 / 频率 / 幅值）
- **OLED 显示**：128×64 OLED 实时显示当前参数

## 硬件平台

| 组件 | 型号 |
|------|------|
| 主控芯片 | STM32F103C8T6 (C8T6 最小系统板) |
| DDS 芯片 | AD9833 (25 MHz 晶振) |
| 显示屏 | 0.96 寸 OLED (128×64, I2C/SPI) |
| 输入设备 | 旋转编码器 (带按键) |

## 引脚分配

| 功能 | 引脚 | 说明 |
|------|------|------|
| AD9833 FSYNC | PB15 | 帧同步 |
| AD9833 SCK | PB14 | 串行时钟 |
| AD9833 DAT | PB13 | 串行数据 |
| AD9833 CS | PB12 | 幅值控制片选 |
| 编码器 A 相 | PB0 | 外部中断线 0 |
| 编码器 B 相 | PB1 | 外部中断线 1 |
| 模式切换按键 | PB10 | 外部中断线 10 |

## 项目结构

```
├── Hardware/          # 硬件驱动层
│   ├── AD9833.c/h     # DDS 波形发生器驱动（软件 SPI）
│   ├── OLED.c/h       # OLED 显示驱动
│   ├── OLED_Font.h    # OLED 字库
│   ├── Encoder.c/h    # 旋转编码器驱动（外部中断）
│   ├── KEY.c/h        # 按键驱动
│   ├── PWM.c/h        # PWM 输出驱动
│   ├── MySPI.c/h      # 软件 SPI 驱动
│   ├── Delay.c/h      # 延时函数（SysTick）
│   ├── IC.c/h         # 输入捕获
│   └── Timer3.c/h     # 定时器3
├── Library/           # STM32 标准外设库 V3.5.0
├── Start/             # 启动文件与 CMSIS 核心
│   ├── startup_stm32f10x_md.s  # 中等容量启动文件
│   ├── stm32f10x.h    # 寄存器定义
│   └── system_stm32f10x.c/h    # 系统时钟配置
├── System/            # 系统级文件
├── User/              # 用户应用层
│   ├── main.c         # 主程序与中断处理
│   ├── stm32f10x_it.c # 中断服务函数
│   └── stm32f10x_conf.h
├── Project.uvprojx    # Keil5 工程文件
├── keilkill.bat       # 清理编译产物脚本
└── README.md
```

## 使用说明

1. 使用 Keil uVision5 打开 `Project.uvprojx`
2. 编译工程（F7）
3. 通过 ST-Link/J-Link 下载到 STM32F103C8T6
4. 上电后 OLED 显示当前参数界面
5. 按编码器按键切换调节模式（波形 → 步长 → 频率 → 幅值 → 循环）
6. 旋转编码器调整当前模式的数值
7. 波形从 AD9833 输出引脚输出

## 操作界面

```
Form: SIN    ← 当前波形（TRI/SIN/SQU）
Step: 10     ← 调节步长
Freq: 1000 Hz ← 当前频率
Amp: 128     ← 当前幅值
```

## 开发环境

- **IDE**: Keil uVision5 (MDK-ARM)
- **芯片库**: STM32F10x Standard Peripheral Library V3.5.0
- **CMSIS**: Cortex-M3 标准
- **编程语言**: C

## 后续计划

- [ ] 修复编码器响应不灵敏问题
- [ ] 扩展频率范围到 mHz 级
- [ ] 添加 ASK/FSK/PSK 调制功能
- [ ] 菜单系统优化
- [ ] 解调功能设计

## License

MIT
