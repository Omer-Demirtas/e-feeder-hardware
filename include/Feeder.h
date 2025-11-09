#ifndef FEEDER_H
#define FEEDER_H

class Feeder {
public:
    virtual ~Feeder() {}

    virtual void init() = 0;

    // Starts the dispensing process. This is non-blocking.
    virtual void startDispensing(int amount) = 0;

    // Must be called repeatedly in the main loop. Handles the timing.
    virtual void update() = 0;

    // Returns true if the feeder is currently in the middle of dispensing.
    virtual bool isBusy() = 0;
    virtual bool isIdle() = 0;
};

#endif