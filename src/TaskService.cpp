#include "TaskService.h"
#include <TimeLib.h>

TaskService::TaskService(StorageService* storage) : _storage(storage) { }

void TaskService::begin(TaskTriggerCallback callback) {
    _tasks = _storage->loadTasks();
    _onTaskTriggeredCallback = callback; 
    Serial.printf("loaded(%d) task successfully!\n", _tasks.size());
}

void TaskService::update() {
    if (millis() - _lastCheckTime < 1000) return;
    _lastCheckTime = millis();

    if (minute() == _lastCheckedMinute) return;
    _lastCheckedMinute = minute();

    for (int i = 0; i < _tasks.size(); ++i) {
        if (_tasks[i].getHour() == hour() && _tasks[i].getMinute() == minute()) {
            if (_onTaskTriggeredCallback) {
                _onTaskTriggeredCallback(_tasks[i]);
            }
        }
    }
}

const std::vector<Task>& TaskService::getTasks() const {
    return _tasks;
}

void TaskService::addTask(const Task& task) {
    _tasks.push_back(task);
    _storage->saveTasks(_tasks);
    Serial.println("New task added");
}

void TaskService::handleTask(const Task& task) {
    Serial.printf("Handling Task(%S) /n", task.getID());
    _onTaskTriggeredCallback(task);
}

void TaskService::deleteTask(const String& taskID) {
    // TODO
}

void TaskService::deleteAllTasks(){
    // TODO
}