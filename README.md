# Intelligent Energy Management System (IEMS)
> ⚠️ **Currently in Deployment Phase**
> - Some features are implemented as placeholders
> - Ongoing development and optimization 

Team: Teenage Engineering Works

Team Members: Pavan Kalsariya, Dhruv Suthar, Pratham Patel, Rishav Patra, Adarsh Singh, Hena Patel

Version: 4.1 (22 FEB)



![IEMS DEMO VIDEO](https://github.com/user-attachments/assets/54c72df2-a1fb-4397-b787-678a62b644d6)

![WhatsApp Image 2025-02-22 at 1 46 34 PM](https://github.com/user-attachments/assets/a58d535c-6a1e-41c2-97d7-b3b1842e2a3c)

## Overview
The **Intelligent Energy Management System (IEMS)** is an advanced energy management solution built on the ESP32 microcontroller. It intelligently manages power distribution between solar, battery, and grid sources while ensuring system safety, efficiency, and reliability. The system is designed for real-time monitoring, control, and optimization of energy usage.

---

## Dependencies
- Wire.h - I2C communication
- OneWire & DallasTemperature - Temperature sensor interface
- Adafruit_INA219 - Current/voltage monitoring
- ZMPT101B - AC voltage measurement
- EmonLib - Energy monitoring
- ESPAsyncWebServer - Web server functionality
- AsyncTCP - Asynchronous TCP operations
- ArduinoJson - JSON data handling

## Key Features
- **Multi-Source Power Management**: Intelligent switching between Solar, Battery, and Grid
- **Real-Time Monitoring**: Temperature, current, voltage, and power consumption tracking
- **Automated Control**: Smart decision-making for optimal power source selection
- **Safety Systems**: 
  - Temperature-based shutdown (>80°C)
  - Overcurrent protection
  - Voltage monitoring
  - Automatic fan control
- **Web Interface**: Modern dashboard for monitoring and control
- **Data Logging**: Comprehensive system metrics recording
- **Fail-Safe Operation**: Degraded mode support for system reliability

## Project Structure
```
├── main.ino          # Main program file
├── config.h          # Configuration and pin definitions
├── webui.h           # Web interface HTML/CSS/JS
├── SensorManager.h   # Sensor data management
├── PowerManager.h    # Power source control
├── SensorInit.h      # Sensor initialization
└── Logger.h          # Logging system
```
---

