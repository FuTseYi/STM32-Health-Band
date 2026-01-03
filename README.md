# STM32 Health Band

<div align="center">

![Version](https://img.shields.io/badge/version-1.0.0-blue.svg)
![License](https://img.shields.io/badge/license-MIT-green.svg)
![Platform](https://img.shields.io/badge/platform-STM32F103-red.svg)
![Language](https://img.shields.io/badge/language-C-blue.svg)
![Status](https://img.shields.io/badge/status-stable-brightgreen.svg)

**A Full-Featured Open Source Smart Health Band Project**

[Features](#-features) • [Quick Start](#-quick-start) • [Hardware](#-hardware-description) • [Development](#-development-documentation) • [Contributing](#-contributing)

[中文](README_zh-CN.md) | **English**

</div>

---
![Star History Chart](https://api.star-history.com/svg?repos=FuTseYi/STM32-Health-Band&type=Date)

## 📖 Project Overview

STM32-Health-Band is an open-source smart health band project based on the **STM32F103C8T6** microcontroller. This project integrates multiple health monitoring functions, including heart rate detection, blood oxygen saturation monitoring, body temperature measurement, pedometer, and fall detection. The device communicates wirelessly with a mobile APP through the ESP8266 WiFi module for real-time health data viewing and remote monitoring.

This project is suitable for embedded system learners, electronics enthusiasts, and engineers interested in wearable device development.

## ✨ Features

### Core Functions
- 🫀 **Heart Rate Monitoring** - Based on MAX30102 sensor using Photoplethysmography (PPG)
- 🩸 **Blood Oxygen Detection** - Real-time SpO2 monitoring
- 🌡️ **Temperature Measurement** - Accurate body temperature monitoring
- 🚶 **Smart Pedometer** - Step counting algorithm based on ADXL345 3-axis accelerometer
- 🚨 **Fall Detection** - Intelligent fall recognition and alarm system
- 📱 **Wireless Connection** - Connect to mobile APP via ESP8266-01S module
- 📺 **Real-time Display** - OLED screen displays health data in real-time
- 🔔 **Audio Alarm** - Buzzer alerts for abnormal conditions

### Technical Features
- ⚡ **Low Power Design** - Optimized power management for extended battery life
- 🔄 **Real-time Monitoring** - 100ms data acquisition cycle, fast response
- 📊 **Data Storage** - Support for historical data caching
- 🌐 **Wireless Transmission** - WiFi real-time data upload
- 🎯 **High Precision Algorithm** - Calibrated sensor algorithms ensure data accuracy

## 🚀 Quick Start

### Hardware Requirements

#### Essential Components
| Component | Model | Quantity | Description |
|-----------|-------|----------|-------------|
| MCU | STM32F103C8T6 | 1 | ARM Cortex-M3, 72MHz |
| Heart Rate & SpO2 Sensor | MAX30102 | 1 | IIC Interface |
| Accelerometer | ADXL345 | 1 | 3-axis accelerometer |
| Display | OLED 128×64 | 1 | SSD1306 driver |
| WiFi Module | ESP8266-01S | 1 | Wireless communication |
| Buzzer | Passive Buzzer | 1 | Alarm notification |
| Debugger | ST-Link V2 | 1 | Program download & debug |

For detailed hardware list and connection instructions, please refer to [HARDWARE.md](HARDWARE.md)

### Software Environment

#### Development Tools
- **IDE**: Keil uVision5 (v5.29 or higher recommended)
- **Compiler**: ARM-MDK V5.06+
- **Download Tool**: STM32 ST-LINK Utility or J-Link
- **Serial Tool**: Any serial debugging assistant (Baud rate 115200)

#### Firmware Library
- STM32F10x Standard Peripheral Library (included in the project)

### Compilation & Flashing

1. **Clone the Project**
   ```bash
   git clone https://github.com/YourUsername/STM32-Health-Band.git
   cd STM32-Health-Band
   ```

2. **Open Project**
   - Open `1、代码/USER/Template.uvprojx` with Keil uVision5

3. **Compile Project**
   - Click `Project` → `Build Target` or press `F7`
   - Ensure no errors or warnings

4. **Flash Program**
   - Connect ST-Link to STM32 development board
   - Click `Flash` → `Download` or press `F8`

### Mobile APP Usage

1. **Install APP**
   - Transfer `4、APP/发布版_手环APP.apk` to Android phone
   - Install APK file (allow installation from unknown sources)

2. **Connect Device**
   - The band will automatically create a WiFi hotspot after power-on
   - Connect phone to device WiFi:
     - **SSID**: `WIFI` / Password: `123456789`
     - Or **SSID**: `www` / Password: `12345678`

3. **View Data**
   - Open the mobile APP to view real-time health data

## 🔧 Hardware Description

### System Architecture

```
┌─────────────────────────────────────────────────┐
│           STM32F103C8T6 Main Controller          │
│            (ARM Cortex-M3, 72MHz)                │
└─────────────────────────────────────────────────┘
         │         │         │         │
    ┌────┴───┐ ┌──┴───┐ ┌──┴───┐ ┌──┴────┐
    │MAX30102│ │ADXL345│ │ OLED │ │ESP8266│
    │HR & SpO2│ │Accel. │ │Display│ │ WiFi  │
    └────────┘ └───────┘ └──────┘ └───────┘
```

### Pin Connections

| STM32 Pin | Connected Device | Function |
|-----------|------------------|----------|
| PB8, PB9 | MAX30102 | IIC (SCL, SDA) |
| PA4, PA5 | ADXL345 | IIC (SCL, SDA) |
| PB6, PB7 | OLED | IIC (SCL, SDA) |
| PA9, PA10 | ESP8266 | UART (TX, RX) |
| PB5 | MAX30102 | Interrupt Input |
| PC13 | Buzzer | GPIO Output |

For complete hardware specifications, see [HARDWARE.md](HARDWARE.md)

## 📂 Project Structure

```
STM32-Health-Band/
├── 1、代码/
│   ├── CORE/              # STM32 core files
│   ├── FWLIB/             # STM32 firmware library
│   ├── HARDWAR/           # Hardware driver layer
│   │   ├── MAX30102.c/h   # Heart rate & SpO2 sensor driver
│   │   ├── adxl345.c/h    # Accelerometer driver
│   │   ├── OLED.c/h       # OLED display driver
│   │   ├── timer.c/h      # Timer driver
│   │   └── IO_Init.c/h    # GPIO initialization
│   ├── SYSTEM/            # System layer code
│   │   ├── delay.c/h      # Delay functions
│   │   ├── sys.c/h        # System configuration
│   │   └── usart.c/h      # Serial communication
│   └── USER/              # User application layer
│       └── main.c         # Main program
├── 2、PCB/                # PCB design files
├── 3、硬件资料/           # Hardware specifications & datasheets
├── 4、APP/                # Android mobile APP
├── 5、原理图/             # Circuit schematics
├── HARDWARE.md            # Hardware detailed documentation
├── DEVELOPMENT.md         # Development documentation
├── CONTRIBUTING.md        # Contributing guidelines
├── LICENSE                # MIT License
├── README.md              # Chinese README
└── README_EN.md           # This file
```

## 💻 Development Documentation

### Core Algorithms

#### Heart Rate Detection Algorithm
Peak detection algorithm analyzes PPG signals from MAX30102 sensor to calculate heart rate:
- Signal preprocessing and filtering
- Peak detection and recognition
- Heart rate calculation (based on RR-Interval)

#### Blood Oxygen Saturation Algorithm
Based on the absorption ratio of red and infrared light:
```
R = (AC_Red / DC_Red) / (AC_IR / DC_IR)
SpO2 = 110 - 25 × R
```

#### Fall Detection Algorithm
Based on 3-axis acceleration data:
```
Total_G = √(X² + Y² + Z²)
Fall Detection: Total_G > 3g or Total_G < 0.5g
```

For detailed development documentation, refer to [DEVELOPMENT.md](DEVELOPMENT.md)

### Performance Specifications

| Specification | Value |
|--------------|-------|
| Heart Rate Range | 60-100 BPM |
| SpO2 Accuracy | ±2% |
| Temperature Accuracy | ±0.5°C |
| Pedometer Accuracy | ≥95% |
| Battery Life | Approx. 6-24 hours |
| WiFi Range | Indoor 10-15 meters |
| Display Refresh Rate | 10Hz |
| Data Acquisition Cycle | 100ms |

## 🤝 Contributing

We welcome all forms of contributions! Whether it's reporting bugs, suggesting new features, or submitting code improvements.

### How to Contribute
1. Fork this repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

For detailed contribution guidelines, see [CONTRIBUTING.md](CONTRIBUTING.md)

### Code Style
- Function naming: lowercase + underscore `sensor_init()`
- Variable naming: lowercase + underscore `sensor_data`
- Macro definition: uppercase + underscore `MAX_BUFFER_SIZE`
- Comments: Use Doxygen style comments

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

You are free to:
- ✅ Commercial use
- ✅ Modification
- ✅ Distribution
- ✅ Private use

With the requirement:
- 📋 Include license and copyright notice

## 👨‍💻 Author

**謝懿Shine** - *Project Creator and Main Maintainer*

## 🙏 Acknowledgments

Thanks to the following open source projects and resources:
- [STM32 Standard Peripheral Library](https://www.st.com/)
- [Keil MDK-ARM](https://www.keil.com/)
- Reference code provided by MAX30102 and ADXL345 sensor manufacturers
- All developers who contributed to this project

## 📞 Contact

- 📧 **Issues**: [Submit Issue](https://github.com/YourUsername/STM32-Health-Band/issues)
- 💬 **Discussions**: [Join Discussion](https://github.com/YourUsername/STM32-Health-Band/discussions)

## 📊 Changelog

### v1.0.0 (2025-01-03)
- ✨ Initial release
- ✅ Implemented heart rate, SpO2, and temperature detection
- ✅ Added pedometer and fall detection features
- ✅ Completed WiFi data transmission
- ✅ Developed companion Android APP
- 📝 Improved project documentation

---

<div align="center">

**⚠️ Disclaimer**

This device is for health monitoring reference and educational purposes only, not for medical diagnosis.  
For health concerns, please consult professional medical institutions.

**Made with ❤️ by 謝懿Shine**

</div>

