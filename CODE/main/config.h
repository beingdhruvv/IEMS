/*
 * Configuration File for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 4
 */

#ifndef CONFIG_H
#define CONFIG_H

// Pin Definitions for ESP32
// ADC1 pins (can be used when WiFi is used)
#define FAN_RELAY_PIN 26        // Cooling Fan Relay (GPIO26)
#define SOLAR_RELAY_PIN 25      // Solar Power Relay (GPIO25)
#define GRID_RELAY_PIN 27       // Grid Power Relay (GPIO32)
#define TEMP_SENSOR_PIN 4       // DS18B20 Temperature Sensor (GPIO4)
#define SOLAR_VOLTAGE_PIN 36    // Solar Voltage Sensor (ADC1_CH0, GPIO36)
#define GRID_VOLTAGE_PIN 39     // Grid Voltage Sensor (ADC1_CH3, GPIO39)
#define I2C_SDA 21              // I2C SDA (GPIO21)
#define I2C_SCL 22              // I2C SCL (GPIO22)
#define SCT013_PIN 35           // SCT-013 Current Sensor (ADC1_CH7, GPIO35)
#define ZMPT101B_PIN 34         // ZMPT101B Voltage Sensor (ADC1_CH6, GPIO34)

// System Parameters
#define CRITICAL_TEMP 80.0     // Critical temperature in Celsius
#define FAN_ON_TEMP 45.0      // Temperature to turn on fan
#define FAN_OFF_TEMP 35.0     // Temperature to turn off fan
#define VOLTAGE_SAMPLES 100    // Number of samples for voltage averaging
#define CURRENT_SAMPLES 100    // Number of samples for current averaging
#define UPDATE_INTERVAL 1000   // Sensor update interval in milliseconds (1 second)

// Safety Thresholds
#define SOLAR_VOLTAGE_MIN 10.0     // Minimum solar voltage
#define SOLAR_VOLTAGE_MAX 14.5     // Maximum solar voltage
#define SOLAR_CURRENT_MAX 30.0     // Maximum solar current
#define OVERCURRENT_DELAY 100      // Delay in ms before overcurrent shutdown
#define TEMP_CHECK_INTERVAL 500    // Temperature check interval in ms
#define VOLTAGE_CHECK_INTERVAL 200 // Voltage check interval in ms

// AC Sensor Calibration
#define ZMPT101B_SENSITIVITY 0.0025  // ZMPT101B sensitivity in V/mV
#define SCT013_CALIBRATION 30        // SCT013 calibration factor
#define ADC_COUNTS 4096              // ESP32 ADC resolution
#define GRID_VOLTAGE_MIN 200.0       // Minimum grid voltage (230V nominal)
#define GRID_VOLTAGE_MAX 250.0       // Maximum grid voltage
#define GRID_CURRENT_MAX 30.0        // Maximum grid current

// WiFi Credentials
#define WIFI_SSID "0000"
#define WIFI_PASSWORD "12121212"

// Voltage and Current Calibration
#define SOLAR_VOLTAGE_FACTOR 0.00489      // 5V / 1024 for 12V system
#define CURRENT_SENSOR_SENSITIVITY 0.185   // V/A for ACS712 30A
#define ADC_REFERENCE 3.3                  // ESP32 ADC reference voltage

// WebSocket Configuration
#define WS_PORT 81
#define WEBSOCKET_PING_INTERVAL 2000
#define DASHBOARD_UPDATE_INTERVAL 1000     // Dashboard update interval (1 second)
#define MAX_GRAPH_POINTS 50               // Maximum points to show on graph

// Logging Configuration
#define LOG_BUFFER_SIZE 256
#define LOG_RATE_LIMIT_MS 100
#define STATUS_UPDATE_INTERVAL 60000  // Status logging interval (1 minute)

// Test Mode Configuration
#define TEST_MODE_ENABLED true        // Enable test mode by default
#define TEST_MODE_DEFAULT_VOLTAGE 13.0 // Default voltage for test mode
#define TEST_MODE_DEFAULT_CURRENT 2.0  // Default current for test mode
#define TEST_MODE_DEFAULT_TEMP 25.0    // Default temperature for test mode

// System States
enum class SystemState {
    NORMAL,
    WARNING,
    CRITICAL,
    EMERGENCY_SHUTDOWN,
    TEST_MODE
};

// Operation Modes
enum class OperationMode {
    NORMAL,
    DEGRADED_NO_SOLAR,
    DEGRADED_GRID_ONLY,
    TEST,
    EMERGENCY
};

// Error Codes
enum class ErrorCode {
    NONE = 0,
    SENSOR_INIT_FAILED = 1,
    TEMPERATURE_HIGH = 2,
    VOLTAGE_OUT_OF_RANGE = 3,
    CURRENT_OUT_OF_RANGE = 4,
    COMMUNICATION_ERROR = 5
};

// Test Mode Settings Structure
struct TestModeSettings {
    bool enabled;
    bool ignoreSensors;
    float solarVoltage;
    float solarCurrent;
    float gridVoltage;
    float gridCurrent;
    float temperature;
};

#endif // CONFIG_H 