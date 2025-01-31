#ifndef SERVOFEEDER_H
#define SERVOFEEDER_H

#include "Feeder.h"

#ifdef ESP32
    #include <ESP32Servo.h>
#else
    #include <Servo.h>
#endif

class ServoFeeder : public Feeder {
private:
    #ifdef ESP32
        Servo servo;  
    #else
        Servo servo;
    #endif    
    
    int pin;

public:
    ServoFeeder(int servoPin);
    void init() override;
    void feed() override;
};

#endif
