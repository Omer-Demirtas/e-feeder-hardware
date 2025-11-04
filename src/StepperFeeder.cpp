#include "StepperFeeder.h"
#include <Arduino.h>

StepperFeeder::StepperFeeder(int pin1, int pin2, int pin3, int pin4) {
    _pins[0] = pin1;
    _pins[1] = pin2;
    _pins[2] = pin3;
    _pins[3] = pin4;
    _state = IDLE;
    _stepsRemaining = 0;
    _currentStepIndex = 0;
    _lastStepTime = 0;
}

void StepperFeeder::init() {
    for (int i = 0; i < 4; i++) {
        pinMode(_pins[i], OUTPUT);
    }
    powerDown(); // Ensure motor is off at start
    Serial.println("StepperFeeder Initialized.");
}

bool StepperFeeder::isBusy() {
    return _state != IDLE;
}

void StepperFeeder::startDispensing(int amount) {
    if (!isBusy()) {
        _stepsRemaining = amount * STEPS_PER_PORTION;
        if (_stepsRemaining > 0) {
            _state = DISPENSING;
            Serial.print("Feeder starting to dispense, steps to go: ");
            Serial.println(_stepsRemaining);
        }
    }
}

void StepperFeeder::update() {
    // Only do work if we are in the DISPENSING state
    if (!isBusy()) {
        return;
    }

    // Check if enough time has passed since the last step (replaces delay())
    if (millis() - _lastStepTime >= _stepDelay) {
        if (_stepsRemaining > 0) {
            stepMotor(); // Perform one step
            _stepsRemaining--;
            _lastStepTime = millis();
        } else {
            // We have completed all steps
            _state = IDLE;
            powerDown(); // Turn off motor coils
            Serial.println("Feeder finished dispensing.");
        }
    }
}

// This private helper performs the logic for a SINGLE step
void StepperFeeder::stepMotor() {
    digitalWrite(_pins[0], _stepSequence[_currentStepIndex][0]);
    digitalWrite(_pins[1], _stepSequence[_currentStepIndex][1]);
    digitalWrite(_pins[2], _stepSequence[_currentStepIndex][2]);
    digitalWrite(_pins[3], _stepSequence[_currentStepIndex][3]);

    // Move to the next step in the sequence
    _currentStepIndex++;
    if (_currentStepIndex >= 8) {
        _currentStepIndex = 0; // Wrap around
    }
}

// Helper to turn off all motor coils
void StepperFeeder::powerDown() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(_pins[i], LOW);
    }
}