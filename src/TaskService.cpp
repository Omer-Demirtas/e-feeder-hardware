#include "Logger.h"
#include <TimeLib.h>
#include "TaskService.h"

TaskService::TaskService(StorageService* storage) : _storage(storage) { }

void TaskService::begin(TaskTriggerCallback callback) {
    _tasks = _storage->loadTasks();
    _onTaskTriggeredCallback = callback; 
    Logger::getInstance().info("loaded(%d) task successfully!\n", _tasks.size());
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
    Logger::getInstance().info("New task added");
}

void TaskService::handleTask(const Task& task) {
    Logger::getInstance().info("Handling Task(%S) /n", task.getID());
    _onTaskTriggeredCallback(task);
}

void TaskService::deleteTask(const String& taskID) {
    for (auto it = _tasks.begin(); it != _tasks.end(); ++it) {
        
        if (it->getID() == taskID) {
            
            _tasks.erase(it);
            
            _storage->saveTasks(_tasks);
            
            Logger::getInstance().info("Task '%s' deleted and storage updated.\n", taskID.c_str());
            
            return;
        }
    }
}

void TaskService::deleteAllTasks(){
    _tasks.clear();
    
    _storage->saveTasks(_tasks);
    
    Logger::getInstance().info("All tasks deleted and storage cleared.");
}