#include "Logger.h"
#include <Arduino.h>
#include "ServoFeeder.h"
#include <TimeLib.h>

ServoFeeder::ServoFeeder(int pin, int startAngle, int feedAngle) {
    _pin = pin;
    _startAngle = startAngle;
    _feedAngle = feedAngle;

    // Initialize state
    _state = IDLE;
    _dispenseDuration = 3000;
}

void ServoFeeder::init() {
     // Move to idle position on startup
    _servo.attach(_pin);
    _servo.write(_startAngle);

    Logger::getInstance().info("Servo feeder initialized, Start Angle %d, Feed Angle %d", _startAngle, _feedAngle);
}

bool ServoFeeder::isBusy() {
    return _state != IDLE;
}

bool ServoFeeder::isIdle() {
    return _state == IDLE;
}

void ServoFeeder::startDispensing(int amount) {
    if (isIdle()) {
        _state = DISPENSING;
        _startTime = millis(); 
        _servo.write(_feedAngle);
        
        Logger::getInstance().info("Feeder (Servo) started dispensing.");
    }
}

void ServoFeeder::update() {
    if (isIdle()) {
        return;
    }

    // Check if the dispensing duration has passed
    if (millis() - _startTime >= _dispenseDuration) {
        // Return servo to idle angle & set state to idle
        _servo.write(_startAngle);

        Logger::getInstance().info("Feeder (Servo) stopped dispensing.");
        
        _state = IDLE;
    }
}