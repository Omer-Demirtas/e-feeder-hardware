#include "DB.hpp";
#include "Arduino.h";
#include "Task.h";
#include <ArduinoJson.h>

DB::DB(String url) {
  url = url;
}

void DB::initDB() 
{
  initTasks();  
}

void DB::initTasks()
{
  DynamicJsonDocument doc(1024);

  deserializeJson(doc, "{\"fields\": {\"07-30-00\": {\"integerValue\": \"100\"}}}");

  JsonObject documentRoot = doc["fields"].as<JsonObject>();

  taskSize = documentRoot.size();
  
  if(documentRoot.size() == 0) return;
  
  tasks =  (Task*)malloc(sizeof(Task) * documentRoot.size());

  int i = 0;
  for (JsonPair keyValue : documentRoot) 
  {
      String key = keyValue.key().c_str();
      int amount = doc["fields"]["17-01-12"]["integerValue"];
    
      tasks[i] = Task(key, amount);
      i++;
  }
}

void DB::printStatus()
{
  char buffer[30];
  
  sprintf(buffer, "tasks from DB = %p", tasks);

  Serial.println(buffer);

  tasks[0].setId(1234);
  //Serial.println(tasks[0].getId());
}
