#pragma once

#include <vector>
#include "Task.h"
#include "StorageService.h"

// Callback func to handle task events
using TaskTriggerCallback = std::function<void(const Task& triggeredTask)>;

class TaskService {
public:
    TaskService(StorageService* storage);

    void begin(TaskTriggerCallback callback);
    
    void update();

    void handleTask(const Task& task);

    void addTask(const Task& task);
    void deleteTask(const String& taskID);
    void deleteAllTasks();
    const std::vector<Task>& getTasks() const;
private:
    StorageService* _storage;
    std::vector<Task> _tasks;

    TaskTriggerCallback _onTaskTriggeredCallback;

    unsigned long _lastCheckTime;
    uint8_t _lastCheckedMinute;
};