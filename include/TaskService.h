#ifndef TASK_SERVICE_H
#define TASK_SERVICE_H

#include "Task.h"
#include <vector>
#include <Arduino.h>
#include "StorageService.h"

class TaskService {
private:
    StorageService& storage; 
    std::vector<Task> tasks;

public:
    TaskService(StorageService& storageService);

    void init(void (*alarmEventCallback)());
    void addTask(Task& task, void (*alarmEventCallback)());
    void deleteTask(String taskId);
    void deleteAllTasks();
    std::vector<Task> getTasks() const;
};

#endif // TASK_SERVICE_H
