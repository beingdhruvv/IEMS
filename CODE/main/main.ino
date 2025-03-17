/*
 * Main Program for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 4
 */

// ESP32 Libraries
#if defined(ESP32)
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif

// Common Libraries
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Project Headers
#include "config.h"
#include "SensorManager.h"
#include "PowerManager.h"
#include "Logger.h"
#include "webui.h"

// Global objects
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
SensorManager sensors;
PowerManager powerManager(sensors);
TestModeSettings testMode = {
    .enabled = TEST_MODE_ENABLED,
    .ignoreSensors = false,
    .solarVoltage = TEST_MODE_DEFAULT_VOLTAGE,
    .solarCurrent = TEST_MODE_DEFAULT_CURRENT,
    .gridVoltage = 230.0,
    .gridCurrent = TEST_MODE_DEFAULT_CURRENT,
    .temperature = TEST_MODE_DEFAULT_TEMP
};

// JSON document for sensor data
StaticJsonDocument<512> jsonDoc;

// Function declarations
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len);
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
void *arg, uint8_t *data, size_t len);
void sendSensorData();
void handleTestMode(const char* command, bool enable);

// Connect to WiFi
void connectWiFi() {
    Logger::log(LogLevel::INFO, "Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Logger::log(LogLevel::INFO, "WiFi Connected! IP: %s", WiFi.localIP().toString().c_str());
    } else {
        Logger::log(LogLevel::ERROR, "WiFi Connection Failed! Restarting...");
        ESP.restart();
    }
}

// WebSocket event handler
void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Logger::log(LogLevel::INFO, "WebSocket client #%u connected from %s", 
                client->id(), client->remoteIP().toString().c_str());
            break;
        case WS_EVT_DISCONNECT:
            Logger::log(LogLevel::INFO, "WebSocket client #%u disconnected", client->id());
            break;
        case WS_EVT_DATA:
            handleWebSocketMessage(arg, data, len);
            break;
        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

// Handle incoming WebSocket messages
void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
    AwsFrameInfo *info = (AwsFrameInfo*)arg;
    if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
        data[len] = 0;
        String message = (char*)data;
        
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, message);
        
        if (!error) {
            const char* command = doc["command"];
            bool enable = doc["enable"] | false;
            
            // Simple direct relay control
            if (strcmp(command, "solar") == 0) {
                digitalWrite(SOLAR_RELAY_PIN, enable ? HIGH : LOW);
                Logger::log(LogLevel::INFO, "Solar relay turned %s", enable ? "ON" : "OFF");
            }
            else if (strcmp(command, "grid") == 0) {
                digitalWrite(GRID_RELAY_PIN, enable ? HIGH : LOW);
                Logger::log(LogLevel::INFO, "Grid relay turned %s", enable ? "ON" : "OFF");
            }
            else if (strcmp(command, "fan") == 0) {
                digitalWrite(FAN_RELAY_PIN, enable ? HIGH : LOW);
                Logger::log(LogLevel::INFO, "Fan relay turned %s", enable ? "ON" : "OFF");
            }
        }
    }
}

// Handle test mode commands
void handleTestMode(const char* command, bool enable) {
    testMode.enabled = enable;
    if (enable) {
        Logger::log(LogLevel::INFO, "Test mode enabled");
    } else {
        Logger::log(LogLevel::INFO, "Test mode disabled");
    }
}

// Send sensor data through WebSocket
// Inside sendSensorData() function
void sendSensorData() {
    static unsigned long lastSend = 0;
    if (millis() - lastSend < 500) {
        return;
    }
    
    if (ws.count() == 0) {
        return;
    }
    
    jsonDoc.clear();
    
    // Temperature data with sensor status check
    if (sensors.isTemperatureSensorOk()) {
        float temp = sensors.getTemperature();
        jsonDoc["temperature"] = temp;  // Send the actual temperature value
        jsonDoc["tempSensorOk"] = true;
    } else {
        jsonDoc["temperature"] = "Sensor Error";  // Indicate error when sensor is not connected
        jsonDoc["tempSensorOk"] = false;
    }
    
    // Fixed values as requested
    jsonDoc["solarPower"] = 0.2;  // Fixed at 0.2 kWh
    jsonDoc["batteryLevel"] = 92; // Fixed at 92%
    
    // Relay states
    jsonDoc["solarEnabled"] = digitalRead(SOLAR_RELAY_PIN) == HIGH;
    jsonDoc["gridEnabled"] = digitalRead(GRID_RELAY_PIN) == HIGH;
    jsonDoc["fanStatus"] = digitalRead(FAN_RELAY_PIN) == HIGH;
    
    // Add timestamp
    jsonDoc["timestamp"] = millis();
    
    String jsonString;
    serializeJson(jsonDoc, jsonString);
    ws.textAll(jsonString);
    lastSend = millis();
    
    // Log temperature for debugging if sensor is okay
    if (sensors.isTemperatureSensorOk()) {
        float temp = sensors.getTemperature();
        Logger::log(LogLevel::DEBUG, "Sent temperature: %.1f°C", temp);
    } else {
        Logger::log(LogLevel::WARN, "Temperature sensor not connected");
    }
}

void setup() {
    // Initialize serial for logging
    Serial.begin(115200);
    Logger::begin(LogLevel::INFO);
    Logger::log(LogLevel::INFO, "IEMS Starting...");
    
    // Initialize temperature sensor pin
    pinMode(TEMP_SENSOR_PIN, INPUT);
    
    // Initialize relay pins
    pinMode(SOLAR_RELAY_PIN, OUTPUT);
    pinMode(GRID_RELAY_PIN, OUTPUT);
    pinMode(FAN_RELAY_PIN, OUTPUT);
    
    // Set initial relay states to OFF
    digitalWrite(SOLAR_RELAY_PIN, LOW);
    digitalWrite(GRID_RELAY_PIN, LOW);
    digitalWrite(FAN_RELAY_PIN, LOW);
    
    Logger::log(LogLevel::INFO, "Relays initialized");
    
    // Initialize temperature sensor
    sensors.begin();
    if (sensors.isTemperatureSensorOk()) {
        Logger::log(LogLevel::INFO, "Temperature sensor initialized successfully");
    } else {
        Logger::log(LogLevel::ERROR, "Failed to initialize temperature sensor");
    }
    
    // Connect to WiFi
    connectWiFi();
    
    // Configure WebSocket
    ws.onEvent(onEvent);
    server.addHandler(&ws);
    
    // Serve web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html);
    });
    
    // Start server
    server.begin();
    Logger::log(LogLevel::INFO, "WebServer started");
}

void loop() {
    static unsigned long lastUpdate = 0;
    static unsigned long lastDashboardUpdate = 0;
    
    // Update sensors more frequently for better temperature readings
    if (millis() - lastUpdate >= 100) {
        sensors.update();
        if (sensors.isTemperatureSensorOk()) {
            float temp = sensors.getTemperature();
            if (temp != DEVICE_DISCONNECTED_C) {
                Logger::log(LogLevel::DEBUG, "Temperature: %.1f°C", temp);
            }
        }
        lastUpdate = millis();
    }
    
    // Update dashboard
    if (millis() - lastDashboardUpdate >= 500) {
        sendSensorData();
        lastDashboardUpdate = millis();
    }
    
    // Clean up disconnected WebSocket clients
    ws.cleanupClients();
}
