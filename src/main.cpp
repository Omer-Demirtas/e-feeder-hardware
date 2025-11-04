#include <WiFi.h>
#include <Arduino.h>
#include <TimeLib.h>

#include "Secrets.h";

#include "TaskService.h"
#include "CommandProcessor.h"
#include "CommunicationInterface.h"
#include "BluetoothInterface.h"

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

BluetoothInterface btInterface("KediMamaMakinesi");

CommunicationInterface* interfaces[] = {
    &btInterface
};

const int NUM_INTERFACES = sizeof(interfaces) / sizeof(interfaces[0]);

Feeder* feeder;

enum AppState { IDLE, WAITING_FOR_FEEDER };

AppState currentState = IDLE;

// =========================================================================
// === CALLBACK METHOD | Called by #taskService
// =========================================================================
void onTaskTriggered_SetState(const Task& triggeredTask) {
    Serial.printf("callback is called for %s %s /n", triggeredTask.getID(), triggeredTask.getTime());

    if (currentState == IDLE) {
        feeder->startDispensing(triggeredTask.getAmount());
        currentState = WAITING_FOR_FEEDER;
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

    Serial.println("System ready to start!");
}


// =========================================================================
// === LOOP
// =========================================================================
void loop() {
    feeder->update();
    taskService.update();

    for (int i = 0; i < NUM_INTERFACES; i++) {
        if (interfaces[i]->commandAvailable()) {
            String command = interfaces[i]->readCommand();
            commandProcessor.process(command, interfaces[i]);
        }
    }

    switch (currentState) {
        case WAITING_FOR_FEEDER: {
            if (!feeder->isBusy()) {
                Serial.println("Motor gorevi tamamladi. Bosa geciliyor.");
                currentState = IDLE;
            }
            break;
        }
        case IDLE: {
            break;
        }
    }
}