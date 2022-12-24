#ifndef TASK_H
#define TASK_H

class Task 
{
  private: 
    int id;
    
  public:
    Task(){};
    Task(int _id);
    int getId() {return id;}
};

#endif
