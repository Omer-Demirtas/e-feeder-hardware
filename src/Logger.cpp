#include "Logger.h"
#include <TimeLib.h>

// Singleton instance getter
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : 
    _logLevel(LOG_LEVEL_DEBUG), 
    _serialEnabled(false) 
{}

void Logger::begin(LogLevel level, bool serialEnabled) {
    setLogLevel(level);
    _serialEnabled = serialEnabled;
}

void Logger::setLogLevel(LogLevel level) {
    _logLevel = level;
}

void Logger::registerObserver(LogObserver* observer) {
    if (observer) {
        _observers.push_back(observer);
    }
}

void Logger::debug(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
}

void Logger::info(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_INFO, format, args);
    va_end(args);
}

void Logger::warn(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_WARN, format, args);
    va_end(args);
}

void Logger::error(const char* format, ...) {
    va_list args;
    va_start(args, format);
    log(LOG_LEVEL_ERROR, format, args);
    va_end(args);
}

void Logger::log(LogLevel level, const char* format, va_list args) {
    if (level < _logLevel) {
        return;
    }

    vsnprintf(_buffer, sizeof(_buffer), format, args);

    const char* levelStr;
    switch (level) {
        case LOG_LEVEL_DEBUG: levelStr = "DEBUG"; break;
        case LOG_LEVEL_INFO:  levelStr = "INFO";  break;
        case LOG_LEVEL_WARN:  levelStr = "WARN";  break;
        case LOG_LEVEL_ERROR: levelStr = "ERROR"; break;
        default:              levelStr = "UNKN";  break;
    }

    unsigned long ms = millis() % 1000;

    char finalMessage[300];

    snprintf(finalMessage, sizeof(finalMessage),
             "[%04d-%02d-%02d %02d:%02d:%02d.%03lu] [%s] %s",
             year(), month(), day(),
             hour(), minute(), second(),
             ms,
             levelStr,
             _buffer);

if (_serialEnabled) {
    Serial.println(finalMessage);
}

    String messageString(finalMessage);
    for (auto& observer : _observers) {
        
        if (observer->isConnected()) {
            observer->onLog(messageString);
        }
    }
}