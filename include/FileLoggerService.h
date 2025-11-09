#pragma once
#include "LogObserver.h"
#include <LittleFS.h>

#define LOG_FILE "/feeder.log"
#define MAX_LOG_SIZE 1024 * 10

class FileLoggerService : public LogObserver {
public:
    FileLoggerService();

    /**
     * @brief Initializes the file system (LittleFS).
     * @return true on success.
     */
    bool begin();

    /**
     * @brief Called by the Logger when a new log is available.
     * Writes the message to the log file.
     */
    void onLog(const String& formattedMessage) override;

    bool isConnected() override;

    /**
     * @brief Reads the entire log file into a String.
     * (CommandProcessor'un 'GETLOGS' komutu için)
     * @return String containing all logs.
     */
    String readLogs();

    /**
     * @brief Deletes the log file.
     * (CommandProcessor'un 'CLEARLOGS' komutu için)
     * @return true on success.
     */
    bool clearLogs();

private:
    /**
     * @brief (Optional) Checks if the log file is too big and rotates it.
     */
    void checkLogRotation();
};