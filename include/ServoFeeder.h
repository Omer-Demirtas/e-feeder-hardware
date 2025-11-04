#ifndef SERVOFEEDER_H
#define SERVOFEEDER_H

#include "Feeder.h"

#ifdef ESP32
    #include <ESP32Servo.h>
#else
    #include <Servo.h>
#endif


class ServoFeeder : public Feeder {
public:
    // Constructor: Takes the pin, idle angle, and dispensing angle
    ServoFeeder(int pin, int startAngle, int feedAngle);

    // Overridden methods from the Feeder interface
    void init() override;
    void startDispensing(int amount) override;
    void update() override;
    bool isBusy() override;

private:
    #ifdef ESP32
        Servo _servo;  
    #else
        Servo _servo;
    #endif    

    // Internal state management
    enum State { IDLE, DISPENSING };
    State _state;

    // Servo configuration
    int _pin;
    int _startAngle;
    int _feedAngle;

    // Non-blocking timer variables
    unsigned long _startTime;
    unsigned long _dispenseDuration;
};

#endif
