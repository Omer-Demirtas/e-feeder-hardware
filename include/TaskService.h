#ifndef TASK_SERVICE_H
#define TASK_SERVICE_H

#include "Task.h"
#include <vector>
#include <Arduino.h>
class TaskService {
private:
    std::vector<Task> tasks;

public:
    TaskService();

    void addTask(Task& task, void (*alarmEventCallback)());
    void deleteTask(String taskId);
    std::vector<Task> getTasks() const;
};

#endif // TASK_SERVICE_H
