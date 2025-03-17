/*
 * Sensor Initialization Implementation for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 4
 */

#include "SensorInit.h"
#include "Logger.h"

SensorInit::SensorInit() : 
    inaSolar(0x40),  // Default INA219 address
    lastRetryTime(0),
    temperature(0),
    zmpt(nullptr),
    oneWire(nullptr),
    tempSensor(nullptr) {
    // Initialize sensor status array to false
    for(int i = 0; i < 4; i++) {
        sensorStatus[i] = false;
    }
}

SensorInit::~SensorInit() {
    if (zmpt) {
        delete zmpt;
        zmpt = nullptr;
    }
    if (oneWire) {
        delete oneWire;
        oneWire = nullptr;
    }
    if (tempSensor) {
        delete tempSensor;
        tempSensor = nullptr;
    }
}

bool SensorInit::begin() {
    // Initialize I2C with specified pins
    Wire.begin(I2C_SDA, I2C_SCL);
    bool allSensorsOk = true;
    
    // Initialize all sensors
    if (!initTemperature()) {
        logSensorError("DS18B20", "Failed to initialize temperature sensor");
        allSensorsOk = false;
    }
    
    if (!initINA219Sensor()) {
        logSensorError("INA219", "Failed to initialize solar sensor");
        allSensorsOk = false;
    }
    
    if (!initACVoltage()) {
        logSensorError("ZMPT101B", "Failed to initialize AC voltage sensor");
        allSensorsOk = false;
    }
    
    if (!initACCurrent()) {
        logSensorError("SCT013", "Failed to initialize AC current sensor");
        allSensorsOk = false;
    }
    
    return allSensorsOk;
}

bool SensorInit::initINA219Sensor() {
    // Initialize Solar INA219
    if (!inaSolar.begin(&Wire)) {
        sensorStatus[1] = false;
        Logger::log(LogLevel::ERROR, "Failed to initialize Solar INA219");
        return false;
    }
    
    sensorStatus[1] = true;
    inaSolar.setCalibration_32V_2A();
    Logger::log(LogLevel::INFO, "Solar INA219 initialized successfully");
    return true;
}

bool SensorInit::initACVoltage() {
    // Create ZMPT101B instance with pin and frequency
    if (zmpt) delete zmpt;
    zmpt = new ZMPT101B(ZMPT101B_PIN, 50); // 50Hz for most countries
    
    if (zmpt) {
        // Set sensitivity and zero point
        zmpt->setSensitivity(ZMPT101B_SENSITIVITY);
        sensorStatus[2] = true;
        return true;
    }
    return false;
}

bool SensorInit::initACCurrent() {
    emon.current(SCT013_PIN, SCT013_CALIBRATION);
    sensorStatus[3] = true;
    return true;
}

bool SensorInit::initTemperature() {
    // Initialize OneWire and DallasTemperature
    if (oneWire) delete oneWire;
    if (tempSensor) delete tempSensor;
    
    oneWire = new OneWire(TEMP_SENSOR_PIN);
    if (!oneWire) {
        Logger::log(LogLevel::ERROR, "Failed to initialize OneWire bus");
        return false;
    }
    
    tempSensor = new DallasTemperature(oneWire);
    if (!tempSensor) {
        Logger::log(LogLevel::ERROR, "Failed to initialize DallasTemperature");
        return false;
    }
    
    tempSensor->begin();
    // Set resolution to 12 bits (0.0625°C) and conversion time to 750ms
    tempSensor->setResolution(12);
    tempSensor->setWaitForConversion(true);
    
    // Request and read initial temperature
    tempSensor->requestTemperatures();
    temperature = tempSensor->getTempCByIndex(0);
    
    if (temperature == DEVICE_DISCONNECTED_C) {
        Logger::log(LogLevel::ERROR, "Temperature sensor not responding");
        sensorStatus[0] = false;
        return false;
    }
    
    Logger::log(LogLevel::INFO, "Temperature sensor initialized successfully. Initial reading: %.1f°C", temperature);
    sensorStatus[0] = true;
    return true;
}

void SensorInit::logSensorError(const char* sensorName, const char* error) {
    Logger::log(LogLevel::ERROR, "Sensor Error: %s - %s", sensorName, error);
}

void SensorInit::update() {
    // Check if it's time to retry failed sensors
    if (millis() - lastRetryTime >= RETRY_INTERVAL) {
        retryFailedSensors();
        lastRetryTime = millis();
    }
    
    // Update temperature reading with proper timing
    static unsigned long lastTempUpdate = 0;
    if (sensorStatus[0] && tempSensor && (millis() - lastTempUpdate >= 100)) {  // Check every 100ms
        tempSensor->requestTemperatures();
        float newTemp = tempSensor->getTempCByIndex(0);
        if (newTemp != DEVICE_DISCONNECTED_C) {
            temperature = newTemp;  // Update temperature without threshold check
            Logger::log(LogLevel::DEBUG, "Temperature reading: %.1f°C", temperature);
            lastTempUpdate = millis();
        } else {
            Logger::log(LogLevel::WARN, "Temperature sensor reading failed");
            sensorStatus[0] = false;
        }
    }
}

void SensorInit::retryFailedSensors() {
    if (!sensorStatus[1]) {
        if (initINA219Sensor()) {
            Logger::log(LogLevel::INFO, "Successfully reinitialized INA219 sensor");
        }
    }
    
    if (!sensorStatus[2]) {
        if (initACVoltage()) {
            Logger::log(LogLevel::INFO, "Successfully reinitialized ZMPT101B sensor");
        }
    }
    
    if (!sensorStatus[0]) {
        if (initTemperature()) {
            Logger::log(LogLevel::INFO, "Successfully reinitialized temperature sensor");
        }
    }
}

bool SensorInit::isOperational(uint8_t sensorIndex) {
    if (sensorIndex < 4) {
        return sensorStatus[sensorIndex];
    }
    return false;
} 