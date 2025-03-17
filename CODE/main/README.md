# Intelligent Energy Management System (IEMS)

## Overview
IEMS is an advanced energy management system built on ESP32 that intelligently manages power distribution between solar, battery, and grid sources while ensuring system safety and efficiency.

## Hardware Requirements
- ESP32 Development Board
- DS18B20 Temperature Sensor
- 2x INA219 Current/Voltage Sensors
- ACS712 Current Sensor
- ZMPT101B AC Voltage Sensor
- SCT013 AC Current Sensor
- 4x Relays for power distribution
- LiFePO4 4S Battery (13V nominal)
- Solar Panel Input (~13V)

## Dependencies
- Wire.h
- OneWire
- DallasTemperature
- Adafruit_INA219
- ZMPT101B
- EmonLib
- ESPAsyncWebServer
- AsyncTCP
- ArduinoJson

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
- Author: Pavan Kalsariya
- Organization: Teenage Engineering Works
- Version: 2.1.0

## License
MIT License
