#pragma once
#include <Arduino.h>
#include "LogObserver.h"
#include <vector>

class Logger {
public:
    /**
     * @brief Log levels to control verbosity of the output.
     */
    enum LogLevel {
        LOG_LEVEL_DEBUG,
        LOG_LEVEL_INFO,
        LOG_LEVEL_WARN,
        LOG_LEVEL_ERROR,
        LOG_LEVEL_NONE // Disables all logging
    };

    /**
     * @brief Gets the single, global instance of the Logger (Singleton pattern).
     * @return Reference to the Logger instance.
     */
    static Logger& getInstance();

    /**
     * @brief Initializes the Logger service.
     * @param level The minimum log level to output (e.g., LOG_LEVEL_INFO).
     * @param serialEnabled true to enable logging to the default Serial port.
     */
    void begin(LogLevel level = LOG_LEVEL_INFO, bool serialEnabled = true);

    /**
     * @brief Sets the minimum log level at runtime.
     * @param level The new minimum log level.
     */
    void setLogLevel(LogLevel level);

    /**
     * @brief Registers a new observer to receive log messages.
     * * Any class that implements the LogObserver interface (like MqttInterface)
     * can be registered here to receive log dispatches.
     * * @param observer A pointer to the observer object.
     */
    void registerObserver(LogObserver* observer);

    // --- Public printf-style logging methods ---

    /**
     * @brief Logs a DEBUG message.
     * @param format The printf-style format string.
     * @param ... Variable arguments for the format string.
     */
    void debug(const char* format, ...);

    /**
     * @brief Logs an INFO message.
     * @param format The printf-style format string.
     * @param ... Variable arguments for the format string.
     */
    void info(const char* format, ...);

    /**
     * @brief Logs a WARNING message.
     * @param format The printf-style format string.
     * @param ... Variable arguments for the format string.
     */
    void warn(const char* format, ...);

    /**
     * @brief Logs an ERROR message.
     * @param format The printf-style format string.
     * @param ... Variable arguments for the format string.
     */
    void error(const char* format, ...);

private:
    Logger();
    ~Logger() {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    /**
     * @brief The internal core function that formats and dispatches the log message
     * to all registered observers.
     * @param level The LogLevel of this specific message.
     * @param format The printf-style format string.
     * @param args The variable argument list.
     */
    void log(LogLevel level, const char* format, va_list args);

    LogLevel _logLevel;
    bool _serialEnabled;
    std::vector<LogObserver*> _observers;

    char _buffer[256]; // Shared buffer for formatting log messages
};