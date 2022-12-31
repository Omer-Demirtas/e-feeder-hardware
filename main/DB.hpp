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
    DB(String _url);
    void initDB();
    void initTasks();
    bool sendLivePing() { return true; };
    void setTaskId(int _index, int _id) { tasks[_index].setId(_id); };
    int getTaskSize() { return taskSize; };
    Task* getTasks() { return tasks; };
    void printStatus();
    String getRequest(String path);
    String getDocument(String documentPath, String mask);
};

#endif
