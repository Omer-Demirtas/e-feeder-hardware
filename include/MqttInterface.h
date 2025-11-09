#pragma once
#include "CommunicationInterface.h"
#include <WiFi.h>
#include <deque>
#include <PubSubClient.h>

class MqttInterface : public CommunicationInterface {
public:
    MqttInterface(
        WiFiClient& client,
        int brokerPort,
        const char* brokerIp, 
        const char* mqttUser, 
        const char* mqttPass
    );

    void begin() override;
    void update() override;
    bool commandAvailable() override;
    String readCommand() override;

    // This should be update to usable version
    void sendResponse(const String& response) override;

    void sendResponse(const char* topic, const String& message, bool retained);
    
    // Log Observer
    bool isConnected() override;
    void onLog(const String& formattedMessage) override;
    
    static const char* TOPIC_LOG;
    static const char* TOPIC_STATE;
    static const char* TOPIC_COMMAND;
private:
    void connectMqtt();
    
    static void onMessageReceived(char* topic, byte* payload, unsigned int length);

    PubSubClient _mqttClient;

    String _brokerIp, _mqttUser, _mqttPass;
    int _brokerPort;

    static std::deque<String> _commandQueue;
};