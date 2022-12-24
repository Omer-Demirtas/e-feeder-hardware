#ifndef DB_H
#define DB_H

#include "Arduino.h";
#include "Task.h";

class DB {
  private:
    String url;
    Task *tasks;
    
  public:
    DB(String url);
    void initDB();
    void sendLivePing();
    Task* getTasks() { return tasks; };
    void printStatus();
};

#endif
