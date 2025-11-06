#include <WiFi.h>
#include <Arduino.h>
#include <TimeLib.h>

#include "Secrets.h"
#include "TaskService.h"
#include <ArduinoJson.h>
#include "MqttInterface.h"
#include "CommandProcessor.h"
#include "BluetoothInterface.h"
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
#define HOME_ASSISTANT_ACTIVE true


// --- NTP Configuration ---
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3 * 3600; // Istanbul is GMT+3, so 3 hours * 3600 seconds/hour TODO make it dynamic
const int daylightOffset_sec = 0;      // Turkey does not observe daylight saving

/**
 * @brief Prints the current local time to the Serial Monitor
 */
void printLocalTime() {
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }

    char timeStringBuff[50]; 
    strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
    Serial.println(timeStringBuff);
}

StorageService storageService;

TaskService taskService(&storageService);

CommandProcessor commandProcessor(&taskService);

WiFiClient globalWifiClient;

BluetoothInterface btInterface("KediMamaMakinesi");

// TODO This should be parametric
MqttInterface mqttInterface(globalWifiClient, MQTT_BROKER_PORT, MQTT_BROKER_IP, MQTT_USER, MQTT_PASS);

CommunicationInterface* interfaces[] = {
    &btInterface,
    &mqttInterface
};

const int NUM_INTERFACES = sizeof(interfaces) / sizeof(interfaces[0]);

Feeder* feeder;

enum DeviceState { IDLE, WAITING_FOR_FEEDER };

DeviceState deviceState;

void setDeviceState(DeviceState state) {
    String stateMessage = (state == IDLE) ? "IDLE" : "FEEDING";
    
    Serial.println("Device status set as " + stateMessage);
    
    deviceState = state;

    // TODO make it better
    if (HOME_ASSISTANT_ACTIVE) {
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

        String jsonOutput;
        serializeJson(jsonDoc, jsonOutput);

        mqttInterface.sendResponse(MqttInterface::TOPIC_STATE, jsonOutput, true);
    }
}

// =========================================================================
// === CALLBACK METHOD | Called by #taskService
// =========================================================================
void onTaskTriggered_SetState(const Task& triggeredTask) {
    Serial.printf("callback is called for %s %s /n", triggeredTask.getID(), triggeredTask.getTime());

    if (deviceState == IDLE) {
        setDeviceState(WAITING_FOR_FEEDER);
        feeder->startDispensing(triggeredTask.getAmount());
    } else {
        Serial.println("Feeder already working!");
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
    Serial.println("WiFi connected!");

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    struct tm timeinfo;

    Serial.print("Waiting for time sync");

    while (!getLocalTime(&timeinfo)) {
        Serial.print(".");
        delay(1000);
    }
    
    Serial.println("/n");

    Serial.println("Time Sync Successfuly!");

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
    Serial.println("\nSistem starting...");

    initWifi();

    #if MOTOR_TYPE == MOTOR_TYPE_SERVO
        feeder = new ServoFeeder(SERVO_PIN, IDLE_ANGLE, FEED_ANGLE);
    #elif MOTOR_TYPE == MOTOR_TYPE_STEPPER
        feeder = new StepperFeeder(IN1, IN2, IN3, IN4);
    #endif

    storageService.begin();

    taskService.begin(onTaskTriggered_SetState);

    feeder->init();

    for (int i = 0; i < NUM_INTERFACES; i++) {
        interfaces[i]->begin();
    }

    printLocalTime();

    setDeviceState(IDLE);

    Serial.printf("Active Interfaces %d \n", NUM_INTERFACES);
    Serial.println("System ready to start!");
}


// =========================================================================
// === LOOP
// =========================================================================
void loop() {
    feeder->update();
    taskService.update();

    for (int i = 0; i < NUM_INTERFACES; i++) {
        interfaces[i]->update();
        
        if (interfaces[i]->commandAvailable()) {
            String command = interfaces[i]->readCommand();
            commandProcessor.process(command, interfaces[i]);
        }
    }

    switch (deviceState) {
        case WAITING_FOR_FEEDER: {
            if (!feeder->isBusy()) {
                setDeviceState(IDLE);
                Serial.println("Motor gorevi tamamladi. Bosa geciliyor.");
            }
            break;
        }
        case IDLE: {
            break;
        }
    }
}