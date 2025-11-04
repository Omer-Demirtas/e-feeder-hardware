#pragma once
#include "Feeder.h"

// Define how many steps make up one standard portion.
// You will need to tune this value by experiment.
const int STEPS_PER_PORTION = 512; // Example: 512 steps is one full revolution for a 28BYJ-48

class StepperFeeder : public Feeder {
public:
    // Constructor takes the four motor control pins
    StepperFeeder(int pin1, int pin2, int pin3, int pin4);

    // Methods from the Feeder interface
    void init() override;
    void startDispensing(int amount) override;
    void update() override;
    bool isBusy() override;

private:
    void stepMotor(); // A private helper to perform a single step
    void powerDown(); // Turns off motor coils to save power

    enum State { IDLE, DISPENSING };
    State _state;

    int _pins[4]; // Stores the motor pins [IN1, IN2, IN3, IN4]
    
    const int _stepSequence[8][4] = {
        {1, 0, 0, 0}, {1, 1, 0, 0}, {0, 1, 0, 0}, {0, 1, 1, 0},
        {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}, {1, 0, 0, 1}
    };

    int _stepsRemaining;      // How many steps are left to perform
    int _currentStepIndex;    // Our current position in the _stepSequence array
    unsigned long _lastStepTime; // When the last step was taken (for timing)
    const int _stepDelay = 2;    // Delay between steps in milliseconds
};