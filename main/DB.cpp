#include "DB.hpp";
#include "Arduino.h";
#include "Task.h";

DB::DB(String url) {
  url = url;
}

void DB::initDB() {
  tasks =  malloc(sizeof(Task) * 4);
  tasks[0] = Task(17, 29, 50);
}

void DB::printStatus()
{
  Serial.println(tasks[0].getId());
}
