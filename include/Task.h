#ifndef TASK_H
#define TASK_H

#include <Arduino.h>

class Task {
    private:
        long id;
        String time;
        uint8_t alarmId;
    public:
        Task(long id, String time) : id(id), time(time) {}

        long getId() { return id; }
        void setId(long id) { this->id = id; }
        uint8_t getAlarmId() { return alarmId; }
        void setAlarmId(uint8_t alarmId) { this->alarmId = alarmId; }
        String getTime() { return time; }
        void setTime(String time) { this->time = time; }
    
        int getHour() {
            return time.substring(0, time.indexOf(':')).toInt();
        }

        int getMinute() {
            int index = time.indexOf(':');
            int secondIndex = time.indexOf(':', index + 1);
            
            return time.substring(index + 1, secondIndex).toInt();
        }
};

#endif