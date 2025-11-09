#include <WiFi.h>
#include "Logger.h"
#include <Arduino.h>
#include <TimeLib.h>
#include "Secrets.h"
#include "TaskService.h"
#include <ArduinoJson.h>
#include "MqttInterface.h"
#include "CommandProcessor.h"
#include "FileLoggerService.h"
#include "BluetoothInterface.h"
#include "CommunicationManager.h"
#include "CommunicationInterface.h"

// =========================================================================
// === MOTOR TİPİ SEÇİMİ ===
// =========================================================================
#define MOTOR_TYPE_SERVO 1
#define MOTOR_TYPE_STEPPER 2

// TODO read it from parameters 
#define MOTOR_TYPE MOTOR_TYPE_SERVO
// =========================================================================


// Seçilen motor tipine göre gerekli dosyaları ve ayarları dahil et
#if MOTOR_TYPE == MOTOR_TYPE_SERVO
    #include "ServoFeeder.h"
    #define SERVO_PIN 13
    #define IDLE_ANGLE 90
    #define FEED_ANGLE 180
#elif MOTOR_TYPE == MOTOR_TYPE_STEPPER
    #include "StepperFeeder.h"
    #define IN1 19
    #define IN2 18
    #define IN3 5
    #define IN4 17
#endif

// TODO make it better, this part should be modified
#define HOME_ASSISTANT_ACTIVE false


// --- NTP Configuration ---
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600; // Istanbul is GMT+3, so 3 hours * 3600 seconds/hour TODO make it dynamic
const int daylightOffset_sec = 0;    // Turkey does not observe daylight saving

WiFiClient globalWifiClient;
FileLoggerService fileLogger;
StorageService storageService;
TaskService taskService(&storageService);
CommandProcessor commandProcessor(&taskService, &fileLogger);

// Communication Interfaces
BluetoothInterface btInterface("KediMamaMakinesi");
MqttInterface mqttInterface(globalWifiClient, MQTT_BROKER_PORT, MQTT_BROKER_IP, MQTT_USER, MQTT_PASS);

CommunicationManager commManager(&commandProcessor);

Feeder* feeder;

enum DeviceState { IDLE, WAITING_FOR_FEEDER };

DeviceState deviceState;

void setDeviceState(DeviceState state) {
    deviceState = state;

    // TODO make it better
    /* if (HOME_ASSISTANT_ACTIVE) {
        String stateMessage = (state == IDLE) ? "IDLE" : "FEEDING";

        // ISO 8601 format (YYYY-MM-DDTHH:MM:SS)
        
        char isoTimestamp[32];
        
        time_t now;
        time(&now);
        
        // format ISO 8601 
        // example: "2025-11-05T21:09:00"
        strftime(isoTimestamp, sizeof(isoTimestamp), "%Y-%m-%dT%H:%M:%SZ", gmtime(&now));

        JsonDocument jsonDoc;
        jsonDoc["state"] = stateMessage;
        jsonDoc["timestamp"] = isoTimestamp;

        JsonArray tasksArray = jsonDoc.createNestedArray("tasks");
        
        const std::vector<Task>& tasks = taskService.getTasks();

        for (const auto& task : tasks) {
            tasksArray.add(task.getTime());
        }

        String jsonOutput;
        serializeJson(jsonDoc, jsonOutput);

        mqttInterface.sendResponse(MqttInterface::TOPIC_STATE, jsonOutput, true);
    } */
}

// =========================================================================
// === CALLBACK METHOD | Called by #taskService
// =========================================================================
void onTaskTriggered_SetState(const Task& triggeredTask) {
    Logger::getInstance().info("callback is called for %s %s", triggeredTask.getID(), triggeredTask.getTime());

    if (deviceState == IDLE) {
        setDeviceState(WAITING_FOR_FEEDER);
        feeder->startDispensing(triggeredTask.getAmount());
    } else {
        Logger::getInstance().warn("Feeder already working!");
    }
}

void initWifi() {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("/n");

    // TODO log more details
    Logger::getInstance().info("WiFi connected!");

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    struct tm timeinfo;

    Logger::getInstance().info("Waiting for time sync");

    while (!getLocalTime(&timeinfo)) {
        Serial.print(".");
        delay(1000);
    }
    
    Serial.println("/n");

    Logger::getInstance().info("Time Sync Successfuly!");

    time_t now_utc;
    time(&now_utc);

    time_t now_local = now_utc + gmtOffset_sec;
    
    setTime(now_local);
}

// =========================================================================
// === SETUP
// =========================================================================
void setup() {
    Serial.begin(115200);
    Logger::getInstance().begin(Logger::LOG_LEVEL_INFO, true);

    if (!fileLogger.begin()) {
        Logger::getInstance().error("Failed to initialize FileLoggerService!");
    } else {
        Logger::getInstance().registerObserver(&fileLogger);
        Logger::getInstance().info("FileLoggerService initialized and registered.");
    }

    #if MOTOR_TYPE == MOTOR_TYPE_SERVO
        feeder = new ServoFeeder(SERVO_PIN, IDLE_ANGLE, FEED_ANGLE);
    #elif MOTOR_TYPE == MOTOR_TYPE_STEPPER
        feeder = new StepperFeeder(IN1, IN2, IN3, IN4);
    #endif

    feeder->init();

    storageService.begin();

    taskService.begin(onTaskTriggered_SetState);

    initWifi();

    commManager.addInterface(&btInterface);
    commManager.addInterface(&mqttInterface);   

    Logger::getInstance().registerObserver(&btInterface);
    Logger::getInstance().registerObserver(&mqttInterface);

    setDeviceState(IDLE);

    btInterface.begin();
    mqttInterface.begin();

    Logger::getInstance().info("System ready to start!");
}


// =========================================================================
// === LOOP
// =========================================================================
void loop() {
    feeder->update();
    taskService.update();

    commManager.updateAll();

    switch (deviceState) {
        case WAITING_FOR_FEEDER: {
            if (feeder->isIdle()) {
                Logger::getInstance().info("Feeder finished!");
                setDeviceState(IDLE);
            }
            break;
        }
        case IDLE: {
            break;
        }
    }
}