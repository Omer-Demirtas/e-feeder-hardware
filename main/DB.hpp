#ifndef DB_H
#define DB_H

#include "Arduino.h";
#include "Task.h";

class DB {
  private:
    int taskSize;
    String url;
    Task *tasks;
    
  public:
    DB(String url);
    void initDB();
    void sendLivePing();
    void setTaskId(int _index, int _id) { tasks[_index].setId(_id); };
    int getTaskSize() { return taskSize; };
    Task* getTasks() { return tasks; };
    void printStatus();
};

#endif
