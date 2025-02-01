#include <queue>
#include "Task.h"
#include <time.h>
#include <WiFi.h>
#include <Arduino.h>
#include <TimeAlarms.h>
#include "ServoFeeder.h"

ServoFeeder feeder(18);

enum FeedAction {
  MOVE_FORWARD,
  MOVE_BACKWARD,
  STOP
};
struct FeedEvent {
  FeedAction action;
  // In milliseconds
  unsigned long duration;
};

std::queue<FeedEvent> eventQueue;

// Adds a movement event, split into multiple steps based on Alarm delay (500ms)
void addMoveEvent(FeedAction action, unsigned long totalDuration) {
  int steps = totalDuration / 500;  // Convert total time into steps of 500ms
  for (int i = 0; i < steps; i++) {
    eventQueue.push({action, 500});
  }
}

// Executes a single event
void executeEvent(FeedEvent event) {
  switch (event.action) {
    case MOVE_FORWARD:
      feeder.startFeed(1);
      Serial.println("Moving forward...");
      break;
    case MOVE_BACKWARD:
      feeder.startFeed(-1);
      Serial.println("Moving backward...");
      break;
    case STOP:
      feeder.stopFeed();
      Serial.println("Feeding done!");
      break;
  }
}

// Processes events from the queue
void processEvents() {
  if (eventQueue.empty()) return;

  // Get next event
  FeedEvent currentEvent = eventQueue.front();
  eventQueue.pop();
  
  // Execute the event
  executeEvent(currentEvent);
}

// Time settings
const char* ntpServer = "pool.ntp.org"; // NTP sunucusu
const long  gmtOffset_sec = 3 * 3600;   // Türkiye için GMT+3
const int   daylightOffset_sec = 0;     // Yaz saati farkı (gerekirse değiştir)

void alarmEvent() {
  Serial.println("Feeding started...");

  // Add events dynamically based on alarm delay (500ms)
  addMoveEvent(MOVE_BACKWARD, 500);
  addMoveEvent(MOVE_FORWARD, 3000);
  addMoveEvent(MOVE_BACKWARD, 500);
  addMoveEvent(MOVE_FORWARD, 3000);
  addMoveEvent(STOP, 500);
}

void initializeTasks() {
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
  // WiFi.begin("SamsungS22", "ieyt9222");

  int networkCount = WiFi.scanNetworks();
  Serial.println("Bulunan ağlar:");

  for (int i = 0; i < networkCount; i++) {
    Serial.printf("SSID: %s, Sinyal Gücü: %d dBm, Kanal: %d\n",
                  WiFi.SSID(i).c_str(),
                  WiFi.RSSI(i),
                  WiFi.channel(i));
  }

  WiFi.begin("TURKSAT-KABLONET-RS0I-2.4G", "606cc36319ed");

  int timeout = 0;

  while (WiFi.status() != WL_CONNECTED && timeout < 20) { // 20 saniye boyunca dene
    delay(1000);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi'ye bağlandı!");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nBağlanamadı! Hata kodu:");
    Serial.println(WiFi.status());
  }

  Serial.println("connected..! ");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());  

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  waitForTimeSync();

  updateTimeFromNTP();

  printCurrentTime();
}

void setup() {
  Task task();

  Serial.begin(115200);
 
  feeder.init();

  wifiConnection();

  initializeTasks();
}

void loop() {
  Alarm.delay(500);
  processEvents();
}