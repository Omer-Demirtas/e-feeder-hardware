#pragma once
#include <Arduino.h>

/**
 * @brief An interface for any class that wants to receive log messages.
 */
class LogObserver {
public:
    virtual ~LogObserver() {}
    
    /**
     * @brief This function is called by the Logger when a new log is created.
     * @param formattedMessage The full log message string.
     */
    virtual void onLog(const String& formattedMessage) = 0;

    /**
     * @brief This function checks the observer ıs connected to any client or topic
     * @return true if the observer is connected and ready.
     */
    virtual bool isConnected() = 0;
};