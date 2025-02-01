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

void ServoFeeder::startFeed(int rotation) {
    if (rotation > 0)
    {
        servo.write(180);
    } else {
        servo.write(0);
    }
}

void ServoFeeder::stopFeed() {
    servo.write(90);
}