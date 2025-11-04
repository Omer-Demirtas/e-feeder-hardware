#include "StorageService.h"
#include <Preferences.h>
#include <ArduinoJson.h>

Preferences preferences;

StorageService::StorageService() {}


/**
 * @brief Bir Task vektörünü, loglamaya uygun bir String'e dönüştürür.
 * * @param tasks Yazdırılacak görev vektörü.
 * @return String "08:30, 14:00, 21:15" formatında.
 */
String tasksToString(const std::vector<Task>& tasks) {
    String logId = "";
    String logString = "";

    for (size_t i = 0; i < tasks.size(); ++i) {
        logString += tasks[i].getID() + "|" + tasks[i].getTime();
    
        if (i < tasks.size() - 1) {
            logString += ", ";
        }
    }
    return logString;
}

/**
 * @brief Preferences (NVS) sistemini başlatır.
 * @return bool Başlatma başarılı ise true, değilse false döner.
 */
bool StorageService::begin() {
    bool success = preferences.begin("tasks", false);

    if (!success) {
        Serial.println("HATA: Preferences (NVS) başlatılamadı!");
    } else {
        Serial.println("Preferences (NVS) başarıyla başlatıldı.");
    }

    return success;
}

String StorageService::getTasks() {
    String jsonString = preferences.getString("tasks");
    
    Serial.println("Getting tasks: " + jsonString);

    return jsonString;
}

void StorageService::saveTasks(const std::vector<Task>& tasks) {
    JsonDocument doc;
    
    for (const auto& task : tasks) {
        JsonObject taskObj = doc.add<JsonObject>();
        taskObj["id"] = task.getID();
        taskObj["time"] = task.getTime();
    }

    String jsonString;
    serializeJson(doc, jsonString);

    preferences.putString("tasks", jsonString);

    Serial.println("Tasks saved to NVS.");
    Serial.println(jsonString);
}

std::vector<Task> StorageService::loadTasks() {
    std::vector<Task> loadedTasks;

    String jsonString = preferences.getString("tasks", "");

    if (jsonString.length() == 0) {
        Serial.println("No saved tasks found in NVS.");
        return loadedTasks;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
        Serial.print("Failed to parse tasks JSON: ");
        Serial.println(error.c_str());
        return loadedTasks;
    }

    JsonArray tasksArray = doc.as<JsonArray>();
    for (JsonObject taskObj : tasksArray) {
        String id = taskObj["id"].as<String>();
        String time = taskObj["time"].as<String>();
        
        loadedTasks.push_back(Task(id, time));
    }
    
    Serial.printf("Loaded %d tasks(%s) from NVS.\n", loadedTasks.size(), tasksToString(loadedTasks).c_str());
    return loadedTasks;
}