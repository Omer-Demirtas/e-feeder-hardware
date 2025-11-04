#include "BluetoothInterface.h"

BluetoothInterface::BluetoothInterface(const char* deviceName) : _deviceName(deviceName) {}

void BluetoothInterface::begin() {
    _serialBT.begin(_deviceName);
}

void BluetoothInterface::update() {
    // BluetoothSerial doesn't need an update call in the loop
}

bool BluetoothInterface::commandAvailable() {
    return _serialBT.available() > 0;
}

String BluetoothInterface::readCommand() {
    return _serialBT.readStringUntil('\n');
}

void BluetoothInterface::sendResponse(const String& response) {
    _serialBT.println(response);
}