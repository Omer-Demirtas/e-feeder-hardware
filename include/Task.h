#pragma once
#include <Arduino.h>

class Task {
public:
    Task(String id = "", String time = "00:00", int amount = 1) {
        _id = id;
        _amount = amount;
        _alarmId = 255; // dtINVALID_ALARM_ID
        
        int colonIndex = time.indexOf(':');
        if (colonIndex > 0) {
            _hour = time.substring(0, colonIndex).toInt();
            _minute = time.substring(colonIndex + 1).toInt();
        } else {
            _hour = 0;
            _minute = 0;
        }
    }

    String getID() const { return _id; }
    String getTime() const { 
        char timeStr[6];
        sprintf(timeStr, "%02d:%02d", _hour, _minute);
        return String(timeStr);
    }
    uint8_t getHour() const { return _hour; }
    uint8_t getMinute() const { return _minute; }
    int getAmount() const { return _amount; }
    
    void setAlarmId(uint8_t alarmId) { _alarmId = alarmId; }
    uint8_t getAlarmId() const { return _alarmId; }

private:
    String _id;
    int _amount;
    uint8_t _hour;
    uint8_t _minute;
    uint8_t _alarmId;
};