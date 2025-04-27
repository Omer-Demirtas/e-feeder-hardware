#ifndef TASK_H
#define TASK_H

#include <Arduino.h>

class Task {
private:
    String id;
    String time;
    uint8_t alarmId;

public:
    Task(String id, String time) : id(id), time(time), alarmId(255) {}

    String getId() const { return id; }
    void setId(String id) { this->id = id; }
    String getTime() const { return time; }
    void setTime(String time) { this->time = time; }
    uint8_t getAlarmId() const { return alarmId; }
    void setAlarmId(uint8_t alarmId) { this->alarmId = alarmId; }

    int getHour() const {
        return time.substring(0, time.indexOf(':')).toInt();
    }

    int getMinute() const {
        int index = time.indexOf(':');
        return time.substring(index + 1).toInt();
    }
};

#endif
