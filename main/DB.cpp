#include "DB.hpp";
#include "Arduino.h";
#include "Task.h";
#include <ArduinoJson.h>
#include <Firebase_ESP_Client.h>
#include <ESP8266WiFi.h>
#include <addons/TokenHelper.h>

#define FIREBASE_PROJECT_ID "feeder-604ff"
#define API_KEY "AIzaSyCkKaNsh61vpZmEKrblsms817lRe1Wg8Ec"
#define USER_EMAIL "test@test.com"
#define USER_PASSWORD "test123"

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

DB::DB(String _url) {
  url = _url;
}

void DB::initDB() 
{
  //config.signer.test_mode = true;
    
  config.api_key = API_KEY;
  
  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  
  config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h
  
  fbdo.setBSSLBufferSize(2048 /* Rx buffer size in bytes from 512 - 16384 */, 2048 /* Tx buffer size in bytes from 512 - 16384 */);
  fbdo.setResponseSize(2048);
  
  Firebase.begin(&config, &auth);
  
  Firebase.reconnectWiFi(true);

  //initTasks(); 
  getDocument("Task/1", "");

}

String DB::getDocument(String documentPath, String mask)
{
    bool isTaskStart = false;
    int lastTaskId = 0;
    tasks  =  (Task*)malloc(sizeof(Task) * 10);
    
    Serial.print("Get a document... ");

    if (Firebase.Firestore.getDocument(&fbdo, FIREBASE_PROJECT_ID, "", documentPath.c_str(), mask.c_str()))
    {
      Serial.printf("ok\n%s\n\n", fbdo.payload().c_str());
      FirebaseJson payload;
      payload.setJsonData(fbdo.payload().c_str());
      
      FirebaseJsonData jsonData;
      payload.get(jsonData, "fields/", true);

      size_t len = payload.iteratorBegin();
      
      FirebaseJson::IteratorValue value;
      
      for (size_t i = 0; i < len; i++)
      {
          value = payload.valueAt(i);
          String key = value.key.c_str();

          Serial.println("key= " + key);
          if(isTaskStart)
          {
            if(key == "integerValue")
            {
              lastTaskId++;
            } else
            {
              Serial.println(String(i) + " " + key.substring(0, 2) + key.substring(3, 5));
              tasks[lastTaskId] = Task(key.substring(0, 2).toInt(), key.substring(3, 5).toInt(), 0);
            }
          } 
          
          if(key == "fields") 
          {
            Serial.println("Taks start tru");
            isTaskStart = true;
          }
          if(key == "createTime")
          {
            Serial.println("Taks start false");
            isTaskStart = false;
          }
 
          Serial.printf("%d, Type: %s, Name: %s, Value: %s\n", i, value.type == FirebaseJson::JSON_OBJECT ? "object" : "array", value.key.c_str(), value.value.c_str());
      }

      taskSize = lastTaskId;

      Serial.println(jsonData.stringValue);
    }
    else
        Serial.println(fbdo.errorReason());
}

String DB::getRequest(String path)
{
  /*    
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
  */
  return "ASD";
}

void DB::initTasks()
{
  /*
  String res = getRequest("http://firestore.googleapis.com/v1/projects/feeder-604ff/databases/(default)/documents/Task/1");

  Serial.print("response");
  Serial.println(res);
  */
  
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
