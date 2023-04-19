#ifndef TASK_H
#define TASK_H

#include "Arduino.h";

class Task 
{
  private: 
    int id;
    int hour;
    int minute;
    int second;
    int amount;
  public:
    Task(){};
    Task(int _hour, int _minute, int _second);
    Task(String _key, int _amount);
    int getId() {return id;};
    void setId(int _id) { id = _id; };    
    int getHour() {return hour;}
    int getMinute() {return minute;}
    int getSecond() {return second;}
    int getAmount() {return amount;}
    void setAmount(int _amount) { amount = _amount; };
    String getTimeString(){ return (String(hour) + "-" + String(minute) + "-" + String(second)); }
};

#endif
