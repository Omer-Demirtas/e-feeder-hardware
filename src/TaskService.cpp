#include "Task.h"
#include <Arduino.h>
#include <HTTPClient.h>
#include <TimeAlarms.h>
#include <ArduinoJson.h>
#include "TaskService.h"

TaskService::TaskService(const String& url) {
    this->apiUrl = url;
}

// Function to URL encode a String
String urlEncode(String str) {
    String encodedString = "";
    char c;
    char buf[4];

    for (size_t i = 0; i < str.length(); i++) {
        c = str.charAt(i);
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encodedString += c;
        } else {
            sprintf(buf, "%%%02X", c);
            encodedString += buf;
        }
    }
    return encodedString;
}

/**
 * Fetchs the device data from server
 * TODO Should be removed this side to general device manager or smtlt
 */
void TaskService::fetchTasks(void (*alarmEventCallback)()) {
    HTTPClient http;

    String url = apiUrl + getLastUpdatedDate();

    http.begin(url);

    int httpCode = http.GET();

    if (httpCode == 200) {
        String payload = http.getString();
        DynamicJsonDocument doc(4096);
        deserializeJson(doc, payload);

        // There is nothing new
        if (payload.length() == 0) {
            Serial.println("There is nothing new");
            return;
        }

        String deviceName = doc["deviceName"].as<String>();
        String lastUpdatedDate = doc["lastUpdatedDate"].as<String>();

        Serial.print("Device Name: ");
        Serial.println(deviceName);

        Serial.print("lastUpdatedDate: ");
        Serial.println(lastUpdatedDate);

        setLastUpdatedDate(lastUpdatedDate);

        if (!tasks.empty()) {
            for (Task& task : tasks) {
                uint8_t alarmId = task.getAlarmId();
                if (alarmId != dtINVALID_ALARM_ID && Alarm.isAllocated(alarmId)) {
                    Serial.print("Removing alarm with ID: ");
                    Serial.println(alarmId);
                    Alarm.free(alarmId);
                } else {
                    Serial.print("Invalid or unallocated alarm ID: ");
                    Serial.println(alarmId);
                }
            }

            // Clear old tasks
            tasks.clear(); 
        } else {
            Serial.println("No tasks available.");
        }

        // Extract feeding schedules
        JsonArray schedulesArray = doc["feedingSchedules"].as<JsonArray>();

        for (JsonObject schedule : schedulesArray) {
            long id = schedule["id"];
            String time = schedule["time"].as<String>();
            
            if (time.isEmpty()) {
                Serial.println("Invalid 'time' in schedule.");
                continue;
            }

            // Create a Task and set alarm
            Task task(id, time);

            // Check if the task hour and minute are valid
            int hour = task.getHour();
            int minute = task.getMinute();

            uint8_t alarmId = Alarm.alarmRepeat(hour, minute, 0, alarmEventCallback);

            if (alarmId == dtINVALID_ALARM_ID) {
                Serial.println("Failed to set alarm.");
                task.setAlarmId(alarmId);
            } else {
                task.setAlarmId(alarmId);

                char message[50];
                sprintf(message, "New alarm set at %s with id %d", task.getTime().c_str(), task.getAlarmId());
                Serial.println(message);

                tasks.push_back(task);
            }
        }

        Serial.println("Schedules loaded successfully.");
    } else {
        Serial.println("Failed to fetch data");
    }
    
    http.end();
}

void TaskService::completeTask(long taskId) {

}