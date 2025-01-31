#ifndef STEPPERFEEDER_H
#define STEPPERFEEDER_H

#include "Feeder.h"

class StepperFeeder : public Feeder {
private:
    //Stepper stepper;
    int stepsPerRevolution;

public:
    StepperFeeder(int steps, int pin1, int pin2, int pin3, int pin4);
    void init() override;
    void feed() override;
};

#endif
