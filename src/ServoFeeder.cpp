#include <Arduino.h>
#include "ServoFeeder.h"

ServoFeeder::ServoFeeder(int servoPin) : pin(servoPin) {}

void ServoFeeder::init() {
    servo.attach(pin);
}

void ServoFeeder::feed() {
    servo.write(180);

    delay(1000);

    servo.write(0);
}

void ServoFeeder::startFeed() {
    servo.write(180);
}

void ServoFeeder::stopFeed() {
    servo.write(90);
}