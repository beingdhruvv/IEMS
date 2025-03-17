/*
 * Sensor Manager for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 4
 */

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "config.h"
#include "SensorInit.h"
#include "Logger.h"

class SensorManager {
private:
    SensorInit sensors;
    float temperature;
    float solarVoltage;
    float solarCurrent;
    float gridVoltage;
    float gridCurrent;
    bool fanStatus;
    bool fanManualMode;
    bool systemShutdown;

    // Private methods
    void checkTemperature();
    void manageFan();
    void readPowerMetrics();

public:
    SensorManager();
    void begin();
    void update();
    bool isCritical() { return systemShutdown; }
    bool isFanOn() { return fanStatus; }
    
    // Getters
    float getTemperature() { return temperature; }
    float getSolarVoltage() { return solarVoltage; }
    float getSolarCurrent() { return solarCurrent; }
    float getGridVoltage() { return gridVoltage; }
    float getGridCurrent() { return gridCurrent; }
    float getSolarPower() { return solarVoltage * solarCurrent; }
    float getGridPower() { return gridVoltage * gridCurrent; }
    
    // Sensor status methods
    bool isTemperatureSensorOk() { return sensors.isTemperatureSensorOk(); }
    bool isSolarSensorOk() { return sensors.isSolarINAOk(); }
    bool isGridSensorOk() { return sensors.isZMPTOk() && sensors.isSCT013Ok(); }
    
    // Fan control
    void setFanManual(bool enable);
    void setFanState(bool on);
    bool isFanManualMode() { return fanManualMode; }
};

#endif // SENSOR_MANAGER_H 