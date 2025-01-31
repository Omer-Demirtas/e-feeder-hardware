#include <time.h>
#include <Arduino.h>

#include "Task.h"
#include <TimeAlarms.h>

#include "ServoFeeder.h"

#include <WiFi.h>

ServoFeeder feeder(18);

bool isFeeding = false;
unsigned long startTime = 0;
const unsigned long feedDuration = 2000; // 10 saniye

const char* ntpServer = "pool.ntp.org"; // NTP sunucusu
const long  gmtOffset_sec = 3 * 3600;   // Türkiye için GMT+3
const int   daylightOffset_sec = 0;     // Yaz saati farkı (gerekirse değiştir)

void alarmEvent() {
  Serial.println("Feeding...");

  feeder.startFeed();

  isFeeding = true;
  startTime = millis();
}

void initializeTasks() {
  // todo
  //Alarm.alarmOnce(15,7,5, alarmEvent);
  //Alarm.alarmOnce(15,7,20, alarmEvent);

  Alarm.alarmOnce(6,00,0, alarmEvent);
  Alarm.alarmOnce(12,00,0, alarmEvent);
  Alarm.alarmOnce(18,00,0, alarmEvent);
  Alarm.alarmOnce(00,00,0, alarmEvent); 
}

void waitForTimeSync() {
  struct tm timeinfo;
  Serial.print("Time Sync: ");
  
  int retry = 0;
  while (!getLocalTime(&timeinfo) && retry < 10) {
    delay(1000);
    retry++;
  }
  
  if (retry >= 10) {
    Serial.print("❌");
  } else {
    Serial.print("✅");
  }
}

void printCurrentTime() {
  Serial.print("Currnet Time: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.println(second());
}

void updateTimeFromNTP() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Time can not found!");
    return;
  }
  
  setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
}

void wifiConnection() {
  //WiFi.begin("TURKSAT-KABLONET-RS0I-2.4G", "606cc36319ed");
  WiFi.begin("SamsungS22", "ieyt9222");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("connected..! ");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());  

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  waitForTimeSync();

  updateTimeFromNTP();

  printCurrentTime();

  Alarm.alarmOnce(hour(), minute(), second() + 10, alarmEvent);
}

void setup() {
  Task task();

  Serial.begin(115200);
 
  feeder.init();

  wifiConnection();

  initializeTasks();
}

void loop() {
  Alarm.delay(1000);

  if (isFeeding && (millis() - startTime >= feedDuration)) {
    feeder.stopFeed();
    isFeeding = false;
  }
}