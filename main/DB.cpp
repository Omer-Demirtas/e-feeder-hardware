#include "DB.hpp";
#include "Arduino.h";
#include "Task.h";

DB::DB(String url) {
  url = url;
}

void DB::initDB() {
  tasks =  malloc(sizeof(Task) * 4);
  tasks[0] = Task(123);
  /*
  Task* _tasks ;
  _tasks[0] = Task(112);
  _tasks[1] = Task(23); 
  _tasks[2] = Task(34); 
  _tasks[3] = Task(457); 

  Serial.println(*tasks);

    tasks = _tasks;

  Serial.println(tasks[0].getId());
*/
}

void DB::printStatus()
{
  Serial.println(tasks[0].getId());
}
