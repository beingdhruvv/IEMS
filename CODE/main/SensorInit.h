/*
 * Sensor Initialization for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 4
 */

#ifndef SENSOR_INIT_H
#define SENSOR_INIT_H

#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Adafruit_INA219.h>
#include <ZMPT101B.h>
#include "EmonLib.h"
#include "config.h"

class SensorInit {
private:
    Adafruit_INA219 inaSolar;      // Solar current/voltage sensor via I2C
    ZMPT101B* zmpt;                // AC voltage sensor
    EnergyMonitor emon;            // AC current sensor (SCT013)
    OneWire* oneWire;              // OneWire bus for temperature sensor
    DallasTemperature* tempSensor; // Temperature sensor
    bool sensorStatus[4];          // Status array for each sensor
    unsigned long lastRetryTime;    // Last sensor retry timestamp
    float temperature;             // Last read temperature
    
    // Retry settings
    static const unsigned long RETRY_INTERVAL = 60000; // 1 minute
    static const uint8_t MAX_RETRIES = 3;
    
    // Private methods
    bool initINA219Sensor();
    bool initACVoltage();
    bool initACCurrent();
    bool initTemperature();
    void logSensorError(const char* sensorName, const char* error);

public:
    SensorInit();
    ~SensorInit();
    bool begin();
    void update();
    bool isOperational(uint8_t sensorIndex);
    void retryFailedSensors();
    
    // Sensor status getters
    bool isTemperatureSensorOk() { return sensorStatus[0]; }
    bool isSolarINAOk() { return sensorStatus[1]; }
    bool isZMPTOk() { return sensorStatus[2]; }
    bool isSCT013Ok() { return sensorStatus[3]; }
    
    // Get sensor objects
    Adafruit_INA219& getSolarINA() { return inaSolar; }
    ZMPT101B& getZMPT() { return *zmpt; }
    EnergyMonitor& getEmon() { return emon; }
    
    // Temperature reading
    float getTemperature() { return temperature; }
};

#endif // SENSOR_INIT_H 