#ifndef TASK_SERVICE_H
#define TASK_SERVICE_H

#include "Task.h"
#include <vector>
#include <Arduino.h>

// TaskService sınıfı
class TaskService {
private:
    String apiUrl;
    std::vector<Task> tasks;
    String lastUpdatedDate = "";

public:
    TaskService(const String& url);
    void fetchTasks(void (*alarmEventCallback)());
    void completeTask(long taskId);

    std::vector<Task> getTasks() const {
        return tasks;
    }

    String getLastUpdatedDate() { return lastUpdatedDate; }
    void setLastUpdatedDate(String lastUpdatedDate) { this->lastUpdatedDate = lastUpdatedDate; }
};

#endif // TASK_SERVICE_H
