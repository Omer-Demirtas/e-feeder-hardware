#pragma once
#include <Arduino.h>

class CommunicationInterface {
public:
    virtual ~CommunicationInterface() {}

    virtual void begin() = 0;
    virtual void update() = 0;
    virtual bool commandAvailable() = 0;
    virtual String readCommand() = 0;
    virtual void sendResponse(const String& response) = 0;
};