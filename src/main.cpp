#include <queue>
#include "Task.h"
#include <time.h>
#include <Arduino.h>
#include <TimeAlarms.h>
#include "ServoFeeder.h"
#include "TaskService.h"
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

#define SLEEP_INTERVAL 500
#define DEVICE_ID "ESP32-12345"

ServoFeeder feeder(18);

TaskService taskService;

enum FeedAction
{
  MOVE_FORWARD,
  MOVE_BACKWARD,
  STOP,
  MOVE_FORWARD_SLIDER,
  MOVE_BACKWARD_SLIDER
};

struct FeedEvent
{
  FeedAction action;
  // In milliseconds
  unsigned long duration;
};

std::queue<FeedEvent> eventQueue;

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
    {1, 0, 0, 1}};

void stepMotor(int stepCount)
{
  int stepIndex = 0;
  int direction = (stepCount > 0) ? 1 : -1;
  stepCount = abs(stepCount);

  Serial.print("stepCount: ");
  Serial.println(stepCount);

  for (int i = 0; i < stepCount; i++)
  {
    digitalWrite(IN1, steps[stepIndex][0]);
    digitalWrite(IN2, steps[stepIndex][1]);
    digitalWrite(IN3, steps[stepIndex][2]);
    digitalWrite(IN4, steps[stepIndex][3]);

    stepIndex += direction;

    if (stepIndex >= 8)
    {
      stepIndex = 0;
    }
    else if (stepIndex < 0)
    {
      stepIndex = 7;
    }

    delay(2);
  }
}

// Adds a movement event, split into multiple steps based on Alarm delay (500ms)
void addMoveEvent(FeedAction action, unsigned long totalDuration)
{
  int steps = totalDuration / 500; // Convert total time into steps of 500ms
  for (int i = 0; i < steps; i++)
  {
    eventQueue.push({action, 500});
  }
}

// Executes a single event
void executeEvent(FeedEvent event)
{
  switch (event.action)
  {
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
    // delay(180000);
    delay(1000);
    stepMotor(-STEP_SIZE * 9);
    break;
  }
}

// Processes events from the queue
void processEvents()
{
  if (eventQueue.empty())
    return;

  // Get next event
  FeedEvent currentEvent = eventQueue.front();
  eventQueue.pop();

  // Execute the event
  executeEvent(currentEvent);
}

void alarmEvent()
{
  Serial.println("Feeding started...");
  SerialBT.println("Feeding started...");

  // Add events dynamically based on alarm delay (500ms)
  addMoveEvent(MOVE_BACKWARD, 500);
  addMoveEvent(MOVE_FORWARD, 3000);
  addMoveEvent(MOVE_BACKWARD, 500);
  addMoveEvent(MOVE_FORWARD, 3000);
  addMoveEvent(STOP, 500);
  addMoveEvent(MOVE_FORWARD_SLIDER, 500);
  addMoveEvent(MOVE_BACKWARD_SLIDER, 500);
}

void printCurrentTime()
{
  Serial.print("Currnet Time: ");
  Serial.print(hour());
  Serial.print(":");
  Serial.print(minute());
  Serial.print(":");
  Serial.println(second());
}

void setSystemTime(const String &datetime)
{
  struct tm t;
  if (sscanf(datetime.c_str(), "%d-%d-%d %d:%d:%d",
             &t.tm_year, &t.tm_mon, &t.tm_mday,
             &t.tm_hour, &t.tm_min, &t.tm_sec) == 6)
  {

    setTime(
        t.tm_hour,
        t.tm_min,
        t.tm_sec,
        t.tm_mday,
        t.tm_mon,
        t.tm_year);

    Serial.println("Saat TimeLib ile başarıyla ayarlandı!");
    Serial.println("Saat TimeLib ile başarıyla ayarlandı!");
  }
  else
  {
    Serial.println("Hatalı tarih formatı! " + datetime);
    SerialBT.println("Hatalı tarih formatı! " + datetime);
  }
}

void setup()
{
  Serial.begin(115200);

  // SLIDER
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  feeder.init();

  SerialBT.begin("ESP32_BT");
}

void loop()
{
  if (SerialBT.available())
  {
    String message = SerialBT.readStringUntil('\n');
    Serial.println("Message: " + message);
    SerialBT.println("Message: " + message);

    if (message.startsWith("SETTIME:"))
    {
      String datetime = message.substring(8); // "2025-04-26 15:45:00" gibi kalır
      setSystemTime(datetime);
    }
    else if (message.startsWith("ADDTASK:"))
    {
      message.remove(0, 8);
      int commaIndex = message.indexOf(',');

      if (commaIndex > 0)
      {
        long id = message.substring(0, commaIndex).toInt();
        String time = message.substring(commaIndex + 1);

        Task newTask(id, time);
        taskService.addTask(newTask, alarmEvent);
      }
    }
    else if (message.startsWith("DELETETASK:"))
    {
      message.remove(0, 10);
      taskService.deleteTask(message);
    } else {
      Serial.println("Unknown command " + message);
      SerialBT.println("Unknown command " + message);
    }
  }

  Alarm.delay(SLEEP_INTERVAL);

  processEvents();
  printCurrentTime();
}