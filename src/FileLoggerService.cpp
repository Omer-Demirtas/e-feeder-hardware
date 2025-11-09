#include "FileLoggerService.h"

/**
 * @brief Initializes the FileLoggerService.
 */
FileLoggerService::FileLoggerService() {}

/**
 * @brief Initializes the LittleFS file system and ensures the log file exists.
 * @return true on success, false on failure.
 */
bool FileLoggerService::begin() {
    if (!LittleFS.begin(true)) { // true = format if mount fails
        Serial.println("LittleFS Mount Failed"); 
        return false;
    }
    File file = LittleFS.open(LOG_FILE, "a"); // Open in append mode (creates if not exists)
    if (!file) {
        Serial.println("Failed to open log file");
        return false;
    }
    file.close();
    return true;
}

/**
 * @brief The file system is always considered "connected" and ready to write.
 * This ensures file logging is always active.
 * @return Always returns true.
 */
bool FileLoggerService::isConnected() {
    return true; 
}

/**
 * @brief Called by the Logger. Appends the formatted log message to the log file.
 * @param formattedMessage The complete log string.
 */
void FileLoggerService::onLog(const String& formattedMessage) {
    checkLogRotation(); // Check if file is too large before writing
    File file = LittleFS.open(LOG_FILE, "a"); // Open in append mode
    if (file) {
        file.println(formattedMessage);
        file.close();
    }
}

/**
 * @brief Reads the entire log file into a single String.
 * Used by CommandProcessor to export logs.
 * @return String containing all logs, or an error message if it fails.
 */
String FileLoggerService::readLogs() {
    File file = LittleFS.open(LOG_FILE, "r"); // Open in read mode
    if (!file) {
        return "Failed to read log file.";
    }
    String logs = file.readString();
    file.close();
    return logs;
}

/**
 * @brief Deletes the log file and recreates an empty one.
 * @return true on success.
 */
bool FileLoggerService::clearLogs() {
    if(LittleFS.remove(LOG_FILE)) {
        // Recreate the empty file
        File file = LittleFS.open(LOG_FILE, "w");
        if (file) {
            file.close();
            return true;
        }
    }
    return false;
}

/**
 * @brief Private helper to check if the log file exceeds MAX_LOG_SIZE.
 * If it does, it deletes the current log and creates a new empty one.
 */
void FileLoggerService::checkLogRotation() {
    File file = LittleFS.open(LOG_FILE, "r");
    if (file && file.size() > MAX_LOG_SIZE) {
        file.close();
        LittleFS.remove(LOG_FILE);
        File newFile = LittleFS.open(LOG_FILE, "w");
        if(newFile) {
            newFile.println("--- Log file rotated (cleared) ---");
            newFile.close();
        }
    } else if (file) {
        file.close();
    }
}