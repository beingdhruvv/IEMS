/*
 * Sensor Manager Implementation for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 5
 */

 #include "SensorManager.h"

 SensorManager::SensorManager() : 
     temperature(0),
     solarVoltage(0),
     solarCurrent(0),
     gridVoltage(0),
     gridCurrent(0),
     fanStatus(false),
     fanManualMode(false),
     systemShutdown(false) {
 }
 
 void SensorManager::begin() {
     // Initialize sensors
     if (!sensors.begin()) {
         Logger::log(LogLevel::ERROR, "Failed to initialize one or more sensors");
     }
     
     // Initialize pins
     pinMode(FAN_RELAY_PIN, OUTPUT);
     digitalWrite(FAN_RELAY_PIN, LOW);
     
     Logger::log(LogLevel::INFO, "SensorManager initialized");
 }
 
 void SensorManager::checkTemperature() {
     if (sensors.isTemperatureSensorOk()) {
         float newTemp = sensors.getTemperature();
         if (newTemp != DEVICE_DISCONNECTED_C) {
             temperature = newTemp;
             
             // Check for critical temperature (80°C)
             if (temperature >= CRITICAL_TEMP && !systemShutdown) {
                 systemShutdown = true;
                 Logger::log(LogLevel::ERROR, "Critical temperature reached: %.1f°C - System shutdown", temperature);
             }
             // Reset shutdown if temperature is safe (5°C hysteresis)
             else if (temperature < (CRITICAL_TEMP - 5) && systemShutdown) {
                 systemShutdown = false;
                 Logger::log(LogLevel::INFO, "Temperature normalized: %.1f°C - System restart allowed", temperature);
             }
         } else {
             Logger::log(LogLevel::WARN, "Temperature sensor disconnected");
         }
     } else {
         Logger::log(LogLevel::WARN, "Temperature sensor not operational");
         temperature = DEVICE_DISCONNECTED_C;
         // Optionally try to read again
         temperature = sensors.getTemperature();
         
         if (temperature >= CRITICAL_TEMP && !systemShutdown) {
             systemShutdown = true;
             Logger::log(LogLevel::ERROR, "Critical temperature reached: %.1f°C - System shutdown", temperature);
         }
         else if (temperature < (CRITICAL_TEMP - 5) && systemShutdown) {
             systemShutdown = false;
             Logger::log(LogLevel::INFO, "Temperature normalized: %.1f°C - System restart allowed", temperature);
         }
     }
 }
 
 void SensorManager::manageFan() {
     if (fanManualMode) {
         return; // Skip automatic control if in manual mode
     }
     
     if (temperature >= FAN_ON_TEMP && !fanStatus) {
         digitalWrite(FAN_RELAY_PIN, HIGH);
         fanStatus = true;
         Logger::log(LogLevel::INFO, "Fan turned ON at %.1f°C", temperature);
     }
     else if (temperature <= FAN_OFF_TEMP && fanStatus) {
         digitalWrite(FAN_RELAY_PIN, LOW);
         fanStatus = false;
         Logger::log(LogLevel::INFO, "Fan turned OFF at %.1f°C", temperature);
     }
 }
 
 void SensorManager::readPowerMetrics() {
     // Read solar metrics from INA219
     if (sensors.isSolarINAOk()) {
         solarVoltage = sensors.getSolarINA().getBusVoltage_V();
         solarCurrent = sensors.getSolarINA().getCurrent_mA() / 1000.0;
     }
     
     // Read grid metrics
     if (sensors.isZMPTOk() && sensors.isSCT013Ok()) {
         gridVoltage = sensors.getZMPT().getRmsVoltage();
         gridCurrent = sensors.getEmon().calcIrms(1480);
     }
 }
 
 void SensorManager::update() {
     // Update sensor initialization status
     sensors.update();
     
     // Update temperature and manage cooling
     checkTemperature();
     manageFan();
     
     // Read power metrics
     readPowerMetrics();
     
     static unsigned long lastStatusLog = 0;
     if (millis() - lastStatusLog >= STATUS_UPDATE_INTERVAL) {
         Logger::log(LogLevel::DEBUG, "Sensor Status - Temp: %s, Solar: %s, Grid: %s",
             isTemperatureSensorOk() ? "OK" : "FAIL",
             isSolarSensorOk() ? "OK" : "FAIL",
             isGridSensorOk() ? "OK" : "FAIL");
         lastStatusLog = millis();
     }
 }
 
 void SensorManager::setFanManual(bool enable) {
     fanManualMode = enable;
     Logger::log(LogLevel::INFO, "Fan manual mode %s", enable ? "enabled" : "disabled");
 }
 
 void SensorManager::setFanState(bool on) {
     if (on != fanStatus) {
         digitalWrite(FAN_RELAY_PIN, on ? HIGH : LOW);
         fanStatus = on;
         Logger::log(LogLevel::INFO, "Fan turned %s at %.1f°C", on ? "ON" : "OFF", temperature);
     }
 }