/*
 * Power Manager Implementation for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 2
 */

 #include "PowerManager.h"

 PowerManager::PowerManager(SensorManager& sensorManager) : 
     sensors(sensorManager),
     autoMode(false),
     currentSource(PowerSource::NONE),
     currentMode(OperationMode::NORMAL),
     solarEnabled(false),
     gridEnabled(false) {
 }
 
 void PowerManager::begin() {
     // Initialize relay pins (battery relay removed)
     pinMode(SOLAR_RELAY_PIN, OUTPUT);
     pinMode(GRID_RELAY_PIN, OUTPUT);
     
     digitalWrite(SOLAR_RELAY_PIN, LOW);
     digitalWrite(GRID_RELAY_PIN, LOW);
     
     // Initial state: Grid power enabled
     setGridPower(true);
     Logger::log(LogLevel::INFO, "PowerManager initialized in GRID mode");
 }
 
 void PowerManager::updateRelays() {
     if (!sensors.isCritical()) {
         digitalWrite(SOLAR_RELAY_PIN, solarEnabled ? HIGH : LOW);
         digitalWrite(GRID_RELAY_PIN, gridEnabled ? HIGH : LOW);
     }
 }
 
 bool PowerManager::isSourceViable(PowerSource source) {
     switch(source) {
         case PowerSource::SOLAR:
             if (!sensors.isSolarSensorOk()) return false;
             return sensors.getSolarVoltage() >= SOLAR_VOLTAGE_MIN && 
                    sensors.getSolarVoltage() <= SOLAR_VOLTAGE_MAX &&
                    sensors.getSolarCurrent() > 0.1;
             
         case PowerSource::GRID:
             if (!sensors.isGridSensorOk()) return false;
             return sensors.getGridVoltage() >= 200.0 && 
                    sensors.getGridVoltage() <= 250.0;
             
         default:
             return false;
     }
 }
 
 void PowerManager::determineOperationMode() {
     bool solarOk = sensors.isSolarSensorOk() && isSourceViable(PowerSource::SOLAR);
     // Battery not supported; set batteryOk to false.
     bool gridOk = sensors.isGridSensorOk() && isSourceViable(PowerSource::GRID);
     
     if (sensors.isCritical()) {
         currentMode = OperationMode::EMERGENCY;
     }
     else if (solarOk && gridOk) {
         currentMode = OperationMode::NORMAL;
     }
     else if (!solarOk && gridOk) {
         currentMode = OperationMode::DEGRADED_NO_SOLAR;
     }
     else {
         currentMode = OperationMode::EMERGENCY;
     }
 }
 
 void PowerManager::handleEmergencyMode() {
     if (isSourceViable(PowerSource::GRID)) {
         setSolarPower(false);
         setGridPower(true);
         Logger::log(LogLevel::WARN, "Emergency mode: Switching to grid power only");
     } else {
         setSolarPower(false);
         setGridPower(false);
         Logger::log(LogLevel::ERROR, "Emergency mode: All power sources disabled");
     }
 }
 
 void PowerManager::optimizePowerSource() {
     if (!autoMode || currentMode == OperationMode::EMERGENCY) return;
 
     switch(currentMode) {
         case OperationMode::NORMAL:
             if (isSourceViable(PowerSource::SOLAR)) {
                 setSolarPower(true);
                 setGridPower(false);
             }
             else {
                 setSolarPower(false);
                 setGridPower(true);
             }
             break;
         case OperationMode::DEGRADED_NO_SOLAR:
             setSolarPower(false);
             setGridPower(true);
             break;
         default:
             handleEmergencyMode();
             break;
     }
 }
 
 void PowerManager::logStateChange(const char* component, bool newState) {
     Logger::log(LogLevel::INFO, "Power state change: %s -> %s", 
                 component, newState ? "ON" : "OFF");
 }
 
 void PowerManager::enableAutoMode(bool enable) {
     autoMode = enable;
     Logger::log(LogLevel::INFO, "Auto mode %s", enable ? "enabled" : "disabled");
     if (enable) {
         optimizePowerSource();
     }
 }
 
 void PowerManager::setSolarPower(bool enable) {
     if (!sensors.isCritical() && enable != solarEnabled) {
         solarEnabled = enable;
         if (enable) {
             currentSource = PowerSource::SOLAR;
         }
         logStateChange("Solar", enable);
         updateRelays();
     }
 }
 
 void PowerManager::setGridPower(bool enable) {
     if (!sensors.isCritical() && enable != gridEnabled) {
         gridEnabled = enable;
         if (enable) {
             currentSource = PowerSource::GRID;
         }
         logStateChange("Grid", enable);
         updateRelays();
     }
 }
 
 float PowerManager::calculateEfficiency() {
     float efficiency = 0.0;
     
     switch(currentSource) {
         case PowerSource::SOLAR:
             efficiency = 90.0;
             if (sensors.getSolarVoltage() >= 13.0) efficiency += 5.0;
             break;
         case PowerSource::GRID:
             efficiency = 75.0;
             break;
         default:
             efficiency = 0.0;
     }
     
     float temp = sensors.getTemperature();
     if (temp > 35.0) {
         efficiency -= (temp - 35.0) * 2.0;
     }
     
     if (currentMode != OperationMode::NORMAL) {
         efficiency -= 10.0;
     }
     
     return constrain(efficiency, 0.0, 100.0);
 }
 
 const char* PowerManager::getOperationModeString() {
     switch(currentMode) {
         case OperationMode::NORMAL: return "Normal";
         case OperationMode::DEGRADED_NO_SOLAR: return "Degraded (No Solar)";
         case OperationMode::EMERGENCY: return "Emergency";
         default: return "Unknown";
     }
 }
 
 void PowerManager::update() {
     determineOperationMode();
     
     if (currentMode == OperationMode::EMERGENCY) {
         handleEmergencyMode();
     }
     else if (autoMode) {
         optimizePowerSource();
     }
     
     updateRelays();
 }