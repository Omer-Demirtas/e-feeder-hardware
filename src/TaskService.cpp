#include <Arduino.h>
#include <TimeAlarms.h>
#include "TaskService.h"
#include <BluetoothSerial.h>

extern BluetoothSerial SerialBT; 

TaskService::TaskService() {}

void TaskService::addTask(Task& task, void (*alarmEventCallback)()) {
    // Alarm ekleme işlemi
    int hour = task.getHour();
    int minute = task.getMinute();

    uint8_t alarmId = Alarm.alarmRepeat(hour, minute, 0, alarmEventCallback);
    if (alarmId != dtINVALID_ALARM_ID) {
        // Alarm başarıyla eklenmiş
        task.setAlarmId(alarmId);  // Alarm ID'sini Task'a kaydediyoruz
        tasks.push_back(task);      // Task'ı listeye ekliyoruz

        Serial.printf("Task eklendi: ID=%ld, Time=%s\n", task.getId().c_str(), task.getTime().c_str());
        SerialBT.printf("Task eklendi: ID=%ld, Time=%s\n", task.getId().c_str(), task.getTime().c_str());
    } else {
        Serial.println("Alarm eklenirken hata oluştu. " + task.getId() + " " + task.getTime());
        SerialBT.println("Alarm eklenirken hata oluştu. " + task.getId() + " " + task.getTime());
    }
}

void TaskService::deleteTask(String taskId) {
    // Task'ı arıyoruz
    for (auto it = tasks.begin(); it != tasks.end(); ++it) {
        if (it->getId() == taskId) {  // Task ID'si eşleşiyorsa
            uint8_t alarmId = it->getAlarmId();
            if (alarmId != dtINVALID_ALARM_ID && Alarm.isAllocated(alarmId)) {
                // Alarmı serbest bırakıyoruz
                Alarm.free(alarmId);
                Serial.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
                SerialBT.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
            }

            // Task'ı siliyoruz
            tasks.erase(it);
            Serial.printf("Task silindi: ID=%ld\n", taskId);
            SerialBT.printf("Task silindi: ID=%ld\n", taskId);

            return;
        }
    }

    Serial.println("Task bulunamadı.");
}

std::vector<Task> TaskService::getTasks() const {
    return tasks;
}
