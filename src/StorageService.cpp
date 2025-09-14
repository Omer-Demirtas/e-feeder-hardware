#include "StorageService.h"
#include <Preferences.h>
#include <ArduinoJson.h>

// Preferences (NVS) nesnesini oluştur
Preferences preferences;

StorageService::StorageService() {}

/**
 * @brief Preferences (NVS) sistemini başlatır.
 * @return bool Başlatma başarılı ise true, değilse false döner.
 */
bool StorageService::begin() {
    // "task-storage" adında bir 'namespace' (alan) açarak NVS'i başlatır.
    // İkinci parametre 'false', alanın okuma/yazma modunda açılacağını belirtir.
    // Eğer bu alana daha önce hiç yazılmamışsa, otomatik olarak oluşturulur.
    bool success = preferences.begin("tasks", false);

    // Başlatma işleminin başarılı olup olmadığını kontrol et.
    if (!success) {
        // Eğer NVS başlatılamazsa, seri porta bir hata mesajı yazdır.
        // Bu genellikle NVS belleği doluysa veya bozulmuşsa olur.
        Serial.println("HATA: Preferences (NVS) başlatılamadı!");
    } else {
        Serial.println("Preferences (NVS) başarıyla başlatıldı.");
    }

    // İşlemin sonucunu (true veya false) döndür.
    return success;
}

String StorageService::getTasks() {
    String jsonString = preferences.getString("tasks", "");
    
    Serial.println(jsonString);

    return jsonString;
}

void StorageService::saveTasks(const std::vector<Task>& tasks) {
    // Görev listesini JSON formatına dönüştürmek için bir JSON dokümanı oluşturuyoruz.
    JsonDocument doc;
    
    // Görevleri bir JSON dizisine ekliyoruz.
    for (const auto& task : tasks) {
        JsonObject taskObj = doc.add<JsonObject>();
        // Sadece ID ve Zaman bilgilerini kaydediyoruz.
        taskObj["id"] = task.getId();
        taskObj["time"] = task.getTime();
    }

    // JSON dokümanını bir String'e çeviriyoruz.
    String jsonString;
    serializeJson(doc, jsonString);

    // JSON String'ini NVS'e "tasks" anahtarıyla kaydediyoruz.
    preferences.putString("tasks", jsonString);

    Serial.println("Tasks saved to NVS.");
    Serial.println(jsonString);
}

std::vector<Task> StorageService::loadTasks() {
    std::vector<Task> loadedTasks;

    // NVS'ten "tasks" anahtarıyla JSON String'ini okuyoruz.
    String jsonString = preferences.getString("tasks", "");

    // Eğer String boşsa, kayıtlı görev yok demektir.
    if (jsonString.length() == 0) {
        Serial.println("No saved tasks found in NVS.");
        return loadedTasks;
    }

    // JSON String'ini deserialize ediyoruz.
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error) {
        Serial.print("Failed to parse tasks JSON: ");
        Serial.println(error.c_str());
        return loadedTasks;
    }

    // JSON dizisindeki her bir objeyi Task nesnesine çeviriyoruz.
    JsonArray tasksArray = doc.as<JsonArray>();
    for (JsonObject taskObj : tasksArray) {
        String id = taskObj["id"].as<String>();
        String time = taskObj["time"].as<String>();
        
        // Okunan bilgilerle yeni bir Task nesnesi oluşturuyoruz.
        loadedTasks.push_back(Task(id, time));
    }
    
    Serial.printf("Loaded %d tasks from NVS.\n", loadedTasks.size());
    return loadedTasks;
}

