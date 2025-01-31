#ifndef FEEDER_H
#define FEEDER_H

class Feeder {
public:
    virtual ~Feeder() = default; 
    virtual void init() = 0;
    virtual void feed() = 0;
};

#endif