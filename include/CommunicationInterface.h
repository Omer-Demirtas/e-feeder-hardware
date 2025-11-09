#pragma once
#include <Arduino.h>
#include "LogObserver.h"

class CommunicationInterface : public LogObserver {
public:
    virtual ~CommunicationInterface() {}

    virtual void begin() = 0;
    virtual void update() = 0;
    virtual bool commandAvailable() = 0;
    virtual String readCommand() = 0;
    virtual void sendResponse(const String& response) = 0;

    // Log observer
    virtual void onLog(const String& formattedMessage) override = 0;
    virtual bool isConnected() override = 0;
};