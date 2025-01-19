#include "Feeder.h"
#include <Arduino.h>
#include "MotorPins.h"

// TODO should be updated when circut created
void Feeder::init() {
    pinMode(MotorPins::STEP_PIN, OUTPUT);
    pinMode(MotorPins::DIR_PIN, OUTPUT);
    pinMode(MotorPins::EN_PIN, OUTPUT);

    digitalWrite(MotorPins::EN_PIN, LOW); 
    digitalWrite(MotorPins::DIR_PIN, LOW);
}

// TODO should be updated when circut created
void Feeder::feed() {
    for (int i = 0; i < 200 * 20; i++) { // 1 tur için 200 adım (1.8 derece/step motor için)
        digitalWrite(MotorPins::STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(MotorPins::STEP_PIN, LOW);
        delayMicroseconds(1000);
    }
}
