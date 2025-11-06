#include "MqttInterface.h"

const char* MqttInterface::TOPIC_STATE = "feeder/state";
const char* MqttInterface::TOPIC_COMMAND = "feeder/command";

std::deque<String> MqttInterface::_commandQueue;

MqttInterface::MqttInterface(
    WiFiClient& client, 
    int brokerPort,
    const char* brokerIp, 
    const char* mqttUser, 
    const char* mqttPass
)
    : _brokerIp(brokerIp), _brokerPort(brokerPort),
      _mqttUser(mqttUser), _mqttPass(mqttPass), _mqttClient(client) 
{
}

void MqttInterface::begin() {
    Serial.println("Begin MqttInterface");
    _mqttClient.setServer(_brokerIp.c_str(), _brokerPort);
    _mqttClient.setCallback(MqttInterface::onMessageReceived);
    connectMqtt();
}

void MqttInterface::update() {
    _mqttClient.loop();
}

bool MqttInterface::commandAvailable() {
    return !_commandQueue.empty();
}

String MqttInterface::readCommand() {
    String command = _commandQueue.front();
    _commandQueue.pop_front();
    return command;
}

void MqttInterface::sendResponse(const String& response) {
    _mqttClient.publish("feeder/status", response.c_str());
}

void MqttInterface::sendResponse(const char* topic, const String& message, bool retained) {
    _mqttClient.publish(topic, message.c_str(), retained);
}

void MqttInterface::connectMqtt() {
    if (_mqttClient.connected()) {
        return;
    }

    Serial.print("connecting to MQTT Server");

    while (!_mqttClient.connected()) {
        Serial.print(".");
        
        // TODO make topic parametric
        // TODO what is mean of id?
        if (_mqttClient.connect("feeder", _mqttUser.c_str(), _mqttPass.c_str())) {
            Serial.println("\nConnection esteblished to MQTT Server!");
            Serial.printf("Subscribing topic(%s) successfully\n", TOPIC_COMMAND);
            _mqttClient.subscribe(TOPIC_COMMAND);
        } else {
            Serial.print("\nERROR, rc=");
            Serial.println(_mqttClient.state());
            delay(5000);
        }
    }
}

// TODO all messages should be a json?
// TODO this should be handled by command processor
void MqttInterface::onMessageReceived(char* topic, byte* payload, unsigned int length) {
    payload[length] = '\0';
    String message = (char*) payload;

    Serial.print("MQTT message recived: [");
    Serial.print(topic);
    Serial.print("]: ");
    Serial.println(message);

    _commandQueue.push_back(message);
}