#ifndef TASK_H
#define TASK_H

class Task 
{
  private: 
    int id;
    int hour,
    int minute;
    int second;
  public:
    Task(){};
    Task(int hour, int minute, int second);
    int getId() {return id;}
};

#endif
