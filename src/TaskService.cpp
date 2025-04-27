#include <Arduino.h>
#include <TimeAlarms.h>
#include "TaskService.h"
#include <BluetoothSerial.h>

extern BluetoothSerial SerialBT; 

TaskService::TaskService() {}

void TaskService::deleteTask(String taskId) {
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->getId() == taskId) {  // Task ID'si eşleşiyorsa
            uint8_t alarmId = it->getAlarmId();
            if (alarmId != dtINVALID_ALARM_ID && Alarm.isAllocated(alarmId)) {
                Alarm.free(alarmId);
                Serial.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
                SerialBT.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
            }

            tasks.erase(it);
            Serial.printf("Task silindi: ID=%ld\n", taskId);
            SerialBT.printf("Task silindi: ID=%ld\n", taskId);

            return;
        }
    }

    Serial.println("Task bulunamadı.");
}

// Delete all tasks
void TaskService::deleteAllTasks() {
    for (auto& task : tasks) {
        uint8_t alarmId = task.getAlarmId();
        if (alarmId != dtINVALID_ALARM_ID && Alarm.isAllocated(alarmId)) {
            Alarm.free(alarmId);
            Serial.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
            SerialBT.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
        }
    }

    tasks.clear();
    Serial.println("Tüm görevler silindi.");
    SerialBT.println("Tüm görevler silindi.");
}

std::vector<Task> TaskService::getTasks() const {
    return tasks;
}
