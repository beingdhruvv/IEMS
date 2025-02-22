# Intelligent Energy Management System (IEMS)

## Overview
ESP32-based energy management system that controls power distribution between solar, grid, and manages system temperature.

## Current Implementation Status

### Implemented Features ✅
- Temperature monitoring with DS18B20 sensor
- Relay control system for:
  - Solar power switching
  - Grid power switching
  - Cooling fan control
- Real-time web dashboard with:
  - Live temperature display
  - Power source status
  - Direct relay controls
  - System monitoring graphs
- Fixed display values:
  - Solar generation: 0.2 kWh
  - Battery level: 92%

### Pending Implementation ⏳
- Real solar power monitoring (INA219)
- Grid power monitoring (ZMPT101B & SCT013)
- Battery management system
- Automatic power source switching
- Data logging system
- Analytics dashboard functionality

## Hardware Setup
- ESP32 Development Board
- DS18B20 Temperature Sensor (GPIO4)
- 3x Relays:
  - Solar Power (GPIO25)
  - Grid Power (GPIO27)
  - Fan Control (GPIO26)

## Dependencies
- ESPAsyncWebServer
- AsyncTCP
- ArduinoJson
- OneWire
- DallasTemperature

## Quick Start
1. Connect hardware according to pin definitions in `config.h`
2. Update WiFi credentials in `config.h`
3. Upload code to ESP32
4. Access dashboard via ESP32's IP address

## Project Structure
```
/main
├── config.h         # Pin definitions & system parameters
├── main.ino        # Main program & WebSocket handling
├── webui.h         # Dashboard interface
├── SensorInit.h/cpp # Sensor initialization
└── SensorManager.h/cpp # Sensor data management
```

## Contributing
Project by Teenage Engineering Works
Author: Pavan Kalsariya

## Features
- Multiple power source management (Solar, Battery, Grid)
- Automatic mode for optimal power source selection
- Real-time temperature monitoring and fan control
- Comprehensive sensor data logging
- Web interface for monitoring and control
- Fail-safe operation with degraded mode support
- Rate-limited logging system

## Installation
1. Clone the repository
2. Install required libraries through Arduino IDE
3. Configure WiFi credentials in config.h
4. Upload to ESP32

## File Structure
- main.ino: Main program file
- config.h: Configuration and pin definitions
- webui.h: Web interface HTML/CSS/JS
- SensorManager.h/cpp: Sensor data management
- PowerManager.h/cpp: Power source control
- SensorInit.h/cpp: Sensor initialization and error handling
- Logger.h/cpp: Logging system

## Usage
1. Power up the system
2. Connect to the ESP32's IP address
3. Monitor and control through the web interface
4. Check serial output for detailed logs

## Safety Features
- Temperature-based shutdown (>80°C)
- Overcurrent protection
- Voltage monitoring
- Automatic fan control

## Development
- Organization: Teenage Engineering Works
- Team Members: Pavan Kalsariya, Pratham Patel, Dhruv Suthar, Hena Patel, Adarsh Singh, Rishav Patra
- Version: 2.1.0

## License
MIT License
