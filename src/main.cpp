#include <queue>
#include "Task.h"
#include <time.h>
#include <WiFi.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <TimeAlarms.h>
#include "ServoFeeder.h"
#include "TaskService.h"

const char* serverUrl = "http://192.168.0.32:8080/device-config?id=6&lastUpdatedDate=";

#define SLEEP_INTERVAL 500
#define DEVICE_ID "ESP32-12345"

unsigned long requestDeley = 1 * 60 * 1000;

ServoFeeder feeder(18);
TaskService taskService(serverUrl);

enum FeedAction {
  MOVE_FORWARD,
  MOVE_BACKWARD,
  STOP,
  MOVE_FORWARD_SLIDER,
  MOVE_BACKWARD_SLIDER
};

struct FeedEvent {
  FeedAction action;
  // In milliseconds 
  unsigned long duration;
};

std::queue<FeedEvent> eventQueue;

unsigned long lastRequestTime;

// Slder

#define IN1 17
#define IN2 16
#define IN3 4
#define IN4 0

#define STEP_SIZE 512

int steps[8][4] = {
  {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};

void stepMotor(int stepCount) {
  int stepIndex = 0;
  int direction = (stepCount > 0) ? 1 : -1;
  stepCount = abs(stepCount);

  Serial.print("stepCount: ");
  Serial.println(stepCount);

  for (int i = 0; i < stepCount; i++) {
    digitalWrite(IN1, steps[stepIndex][0]);
    digitalWrite(IN2, steps[stepIndex][1]);
    digitalWrite(IN3, steps[stepIndex][2]);
    digitalWrite(IN4, steps[stepIndex][3]);

    stepIndex += direction;

    if (stepIndex >= 8) {
      stepIndex = 0;
    } else if (stepIndex < 0) {
      stepIndex = 7;
    }

    delay(2);
  }
}

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
    case MOVE_FORWARD_SLIDER:
      Serial.println("Slider starting");
      delay(1000);
      stepMotor(STEP_SIZE * 9);
      break; 
    case MOVE_BACKWARD_SLIDER:
      Serial.println("Slider backward starting");
      // TODO make it parametric
      //delay(180000);
      delay(1000);
      stepMotor(-STEP_SIZE * 9);
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
  addMoveEvent(MOVE_FORWARD_SLIDER, 500);
  addMoveEvent(MOVE_BACKWARD_SLIDER, 500);
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
  
    setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
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

void wifiConnection() {
  WiFi.begin("TURKSAT-KABLONET-RS0I-2.4G", "606cc36319ed");
  
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(1000);
    Serial.print(".");
    timeout++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("nWiFi connection esteblished!");
    Serial.print("IP Adresi: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Wifi connection error with status code: ");
    Serial.println(WiFi.status());
    return;
  }

  Serial.println("connected..! ");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());  

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  waitForTimeSync();

  printCurrentTime();
}

void setupAlarms() {
  taskService.fetchTasks(alarmEvent);

  lastRequestTime = millis();
}

void setup() {
  Serial.begin(115200);
 
  // SLIDER

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  feeder.init();

  wifiConnection();

  setupAlarms();
}

void loop() {
  Alarm.delay(SLEEP_INTERVAL);

  processEvents();

  unsigned long now = millis();

  if (now - lastRequestTime >= requestDeley) {
    setupAlarms();
  }
}