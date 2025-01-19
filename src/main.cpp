#include <time.h>
#include <TimeAlarms.h>
#include <Arduino.h>
#include "Task.h"

/*
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

void wifiConnection() {
  WiFi.begin("SamsungS22", "ieyt9222");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("connected..! ");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());  
}
*/

void alarmEvent() {
  Serial.println("Alarm");
}

void initializeTasks() {
  Alarm.alarmOnce(7,30,0, alarmEvent);
}

void setup() {
  Task task();

  Serial.begin(9600);

  //wifiConnection();

  // set current time
  setTime(7, 29, 40, 1, 1, 10);

  initializeTasks();
}

void loop() {
  Alarm.delay(1000);
  
  Serial.println(second());
}