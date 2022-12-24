#ifndef TASK_H
#define TASK_H

class Task 
{
  private: 
    int id;
    int hour;
    int minute;
    int second;
  public:
    Task(){};
    Task(int _hour, int _minute, int _second);
    int getId() {return 1;}
    int getHour() {return hour;}
    int getMinute() {return minute;}
    int getSecond() {return second;}

};

#endif
