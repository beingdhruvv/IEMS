/*
 * Logger Implementation for IEMS Project
 * Author: Pavan Kalsariya
 * Teenage Engineering Works
 * Update Counter: 1
 */

#include "Logger.h"

// Initialize static members
LogLevel Logger::currentLevel = LogLevel::INFO;
unsigned long Logger::lastLogTime = 0;

void Logger::begin(LogLevel level) {
    currentLevel = level;
    Serial.begin(115200);
    while(!Serial) delay(10);
    log(LogLevel::INFO, "Logger initialized at level: %s", getLevelString(level));
}

void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
    log(LogLevel::INFO, "Log level changed to: %s", getLevelString(level));
}

const char* Logger::getLevelString(LogLevel level) {
    switch(level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default:             return "UNKNOWN";
    }
}

bool Logger::shouldLog(LogLevel level) {
    return static_cast<int>(level) >= static_cast<int>(currentLevel);
} 