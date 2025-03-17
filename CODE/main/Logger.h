/*
 * Logger for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 1
 */

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

class Logger {
private:
    static LogLevel currentLevel;
    static unsigned long lastLogTime;
    static const unsigned long LOG_INTERVAL = 100; // Minimum time between logs in ms
    
    static const char* getLevelString(LogLevel level);
    static bool shouldLog(LogLevel level);

public:
    static void begin(LogLevel level = LogLevel::INFO);
    static void setLogLevel(LogLevel level);
    
    template<typename... Args>
    static void log(LogLevel level, const char* format, Args... args) {
        if (!shouldLog(level)) return;
        
        unsigned long currentTime = millis();
        if (currentTime - lastLogTime < LOG_INTERVAL) return;
        
        lastLogTime = currentTime;
        
        // Print timestamp
        Serial.print(currentTime);
        Serial.print(" [");
        Serial.print(getLevelString(level));
        Serial.print("] ");
        
        // Print formatted message
        char buffer[256];
        snprintf(buffer, sizeof(buffer), format, args...);
        Serial.println(buffer);
    }
};

#endif // LOGGER_H 