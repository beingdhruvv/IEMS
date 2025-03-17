/*
 * Power Manager for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 2
 */

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include "config.h"
#include "SensorManager.h"
#include "Logger.h"

enum class PowerSource {
    SOLAR,
    BATTERY,
    GRID,
    NONE
};


class PowerManager {
private:
    SensorManager& sensors;
    bool autoMode;
    PowerSource currentSource;
    OperationMode currentMode;
    bool solarEnabled;
    bool batteryEnabled;
    bool gridEnabled;

    void updateRelays();
    void optimizePowerSource();
    bool isSourceViable(PowerSource source);
    void determineOperationMode();
    void handleEmergencyMode();
    void logStateChange(const char* component, bool newState);

public:
    PowerManager(SensorManager& sensorManager);
    void begin();
    void update();
    
    // Control methods
    void enableAutoMode(bool enable);
    void setSolarPower(bool enable);
    void setBatteryPower(bool enable);
    void setGridPower(bool enable);
    
    // Status methods
    bool isAutoModeEnabled() { return autoMode; }
    bool isSolarEnabled() { return solarEnabled; }
    bool isBatteryEnabled() { return batteryEnabled; }
    bool isGridEnabled() { return gridEnabled; }
    PowerSource getCurrentSource() { return currentSource; }
    OperationMode getOperationMode() { return currentMode; }
    
    // System efficiency calculation
    float calculateEfficiency();
    
    // Get operation mode as string
    const char* getOperationModeString();
};

#endif // POWER_MANAGER_H 