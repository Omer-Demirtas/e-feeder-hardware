#ifndef STORAGE_SERVICE_H
#define STORAGE_SERVICE_H

#include <vector>
#include "Task.h"
#include <Arduino.h>

// Bu sınıf, görev listesini ESP32'nin NVS (Non-Volatile Storage) belleğine
// kaydetmek ve oradan okumakla sorumludur.
class StorageService {
public:
    StorageService();
    
    bool begin();
    String getTasks();
    void saveTasks(const std::vector<Task>& tasks);

    std::vector<Task> loadTasks();
};

#endif // STORAGE_SERVICE_H
