#pragma once
#include "CommunicationInterface.h"
#include <BluetoothSerial.h>

class BluetoothInterface : public CommunicationInterface {
public:
    BluetoothInterface(const char* deviceName);
    void begin() override;
    void update() override;
    bool commandAvailable() override;
    String readCommand() override;
    void sendResponse(const String& response) override;
    
    // Log Observer
    bool isConnected() override;
    void onLog(const String& formattedMessage) override;
private:
    BluetoothSerial _serialBT;
    String _deviceName;
};