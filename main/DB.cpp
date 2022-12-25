#include "DB.hpp";
#include "Arduino.h";
#include "Task.h";
#include <ArduinoJson.h>

DB::DB(String url) {
  url = url;
}

void DB::initDB() {

  DynamicJsonDocument doc(1024);

  deserializeJson(doc, "{\"fields\": {\"17-01-12\": {\"integerValue\": \"100\"}}}");

  JsonObject documentRoot = doc["fields"].as<JsonObject>();
  
  if(documentRoot.size() == 0) return;
  
  tasks =  malloc(sizeof(Task) * documentRoot.size());

  int i = 0;
  for (JsonPair keyValue : documentRoot) {
      Serial.println(keyValue.key().c_str());
      
      String key = "17-01-12";
      int amount = doc["fields"]["17-01-12"]["integerValue"];
    
      tasks[i] = Task(key, amount);
      i++;
  }



  //tasks[0] = Task(17, 29, 50);
}

void DB::printStatus()
{
  Serial.println(tasks[0].getId());
}
