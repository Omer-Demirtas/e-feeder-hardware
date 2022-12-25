#ifndef ENGINE_H
#define ENGINE_H

#include <Stepper.h>

class Engine : Stepper {
  private:
    int pin1;
    int pin2;
    int pin3;
    int pin4;
    
  public:
  
    Engine(int p1, int p2, int p3, int p4) : Stepper(2038, p1, p2, p3, p4)
    {
      setSpeed(5);
    };
    void run()
    {
      step(2048);
      delay(1000);
    };
};

#endif
