#include <Arduino.h>
#include "ServoFeeder.h"
#include <TimeLib.h>

ServoFeeder::ServoFeeder(int pin, int startAngle, int feedAngle) {
    _pin = pin;
    _startAngle = startAngle;
    _feedAngle = feedAngle;
    
    Serial.println("Servo feed constructor");

    Serial.println(startAngle);

    _servo.write(startAngle);

    // Initialize state
    _state = IDLE;
    _dispenseDuration = 3000;
}

void ServoFeeder::init() {
    _servo.attach(_pin);
     // Move to idle position on startup
    _servo.write(_startAngle);
    Serial.println("ServoFeeder Initialized.");
}

bool ServoFeeder::isBusy() {
    return _state != IDLE;
}

void ServoFeeder::startDispensing(int amount) {
    if (!isBusy()) {
        _state = DISPENSING;
        _startTime = millis(); 
        _servo.write(_feedAngle);
        
        Serial.println("Feeder (Servo) started dispensing.");

        Serial.print(hour());
        Serial.print(":");
        Serial.print(minute());
        Serial.print(":");
        Serial.print(second());
        Serial.println("");
    }
}

void ServoFeeder::update() {
    if (!isBusy()) {
        return;
    }

    // Check if the dispensing duration has passed
    if (millis() - _startTime >= _dispenseDuration) {
         // Return servo to idle angle & set state to idle
        _servo.write(_startAngle);
        

        Serial.println("Feeder (Servo) started dispensing.");

        Serial.print(hour());
        Serial.print(":");
        Serial.print(minute());
        Serial.print(":");
        Serial.print(second());
        Serial.println("");
    }
}