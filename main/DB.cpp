#include "DB.hpp";
#include "Arduino.h";
#include "Task.h";
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>


DB::DB(String _url) {
  url = _url;
}

void DB::initDB() 
{
  initTasks();  
}

String DB::getRequest(String path)
{
      
  Serial.print("url");
  Serial.println(path);
  WiFiClient client;
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client, path);

  // If you need Node-RED/server authentication, insert user and password below
  //http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

  /*
  http.addHeader("User-Agent", "PostmanRuntime/7.30.0");
  http.addHeader("Accept", "*");
  http.addHeader("Postman-Token", "f0a40145-55b3-4861-b6bb-77e36173258f");
  http.addHeader("Accept-Encoding", "gzip, deflate, br");
  http.addHeader("HOst", "firestore.googleapis.com");
  http.addHeader("Connection", "keep-alive");
  */
  //http.addHeader("", "");

  // Send HTTP POST request
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  for(int i = 0; i< http.headers(); i++){
    Serial.println(http.header(i));
  }

  http.end();

  return payload;
}

void DB::initTasks()
{
  String res = getRequest("http://firestore.googleapis.com/v1/projects/feeder-604ff/databases/(default)/documents/Task/1");

  Serial.print("response");
  Serial.println(res);

  /*
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
  */
}

void DB::printStatus()
{
  char buffer[30];
  
  sprintf(buffer, "tasks from DB = %p", tasks);

  Serial.println(buffer);

  tasks[0].setId(1234);
  //Serial.println(tasks[0].getId());
}
