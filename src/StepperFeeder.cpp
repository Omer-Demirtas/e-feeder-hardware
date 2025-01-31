#include "StepperFeeder.h"
#include <Arduino.h>
#include "MotorPins.h"

StepperFeeder::StepperFeeder(int steps, int pin1, int pin2, int pin3, int pin4) {}

void StepperFeeder::init() {
    pinMode(MotorPins::STEP_PIN, OUTPUT);
    pinMode(MotorPins::DIR_PIN, OUTPUT);
    pinMode(MotorPins::EN_PIN, OUTPUT);

    digitalWrite(MotorPins::EN_PIN, LOW); 
    digitalWrite(MotorPins::DIR_PIN, LOW);
}

void StepperFeeder::feed() {
        digitalWrite(MotorPins::DIR_PIN, HIGH);

    for (int i = 0; i < 200 * 2; i++) { // 1 tur için 200 adım (1.8 derece/step motor için)
        digitalWrite(MotorPins::STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(MotorPins::STEP_PIN, LOW);
        delayMicroseconds(1000);
    }

    digitalWrite(MotorPins::DIR_PIN, LOW);

    for (int i = 0; i < 200 * 5; i++) { // 1 tur için 200 adım (1.8 derece/step motor için)
        digitalWrite(MotorPins::STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(MotorPins::STEP_PIN, LOW);
        delayMicroseconds(1000);
    }

    digitalWrite(MotorPins::DIR_PIN, HIGH);

    for (int i = 0; i < 200 * 2; i++) { // 1 tur için 200 adım (1.8 derece/step motor için)
        digitalWrite(MotorPins::STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(MotorPins::STEP_PIN, LOW);
        delayMicroseconds(1000);
    }

    digitalWrite(MotorPins::DIR_PIN, LOW);

    for (int i = 0; i < 200 * 5; i++) { // 1 tur için 200 adım (1.8 derece/step motor için)
        digitalWrite(MotorPins::STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(MotorPins::STEP_PIN, LOW);
        delayMicroseconds(1000);
    }

    digitalWrite(MotorPins::DIR_PIN, HIGH);

    for (int i = 0; i < 200 * 2; i++) { // 1 tur için 200 adım (1.8 derece/step motor için)
        digitalWrite(MotorPins::STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(MotorPins::STEP_PIN, LOW);
        delayMicroseconds(1000);
    }


    digitalWrite(MotorPins::DIR_PIN, LOW);
        for (int i = 0; i < 200 * 5; i++) { // 1 tur için 200 adım (1.8 derece/step motor için)
        digitalWrite(MotorPins::STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(MotorPins::STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
}
