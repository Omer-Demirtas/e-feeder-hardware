#include <Arduino.h>
#include <TimeAlarms.h>
#include "TaskService.h"
#include <BluetoothSerial.h>

extern BluetoothSerial SerialBT;

/**
 * @brief TaskService'i başlatır. Kayıtlı görevleri NVS'ten yükler ve alarmları yeniden kurar.
 * @param alarmEventCallback Alarm tetiklendiğinde çalıştırılacak olan fonksiyonun adresi.
 */
void TaskService::init(void (*alarmEventCallback)()) {
    tasks = storage.loadTasks();

    setTime(10, 0, 0, 1, 1, 2000);

    if (tasks.size() == 0)
    {
        Serial.printf("No tasks found");
        return;
    }
    

    Serial.printf("%d adet kayitli gorev bulundu. Alarmlar yeniden olusturuluyor...\n", tasks.size());

    // 2. Yüklenen her bir görev için (for-each döngüsü ile) alarmları yeniden oluştur.
    // 'auto& task' kullanıyoruz çünkü task nesnesinin içindeki alarmId'yi güncelleyeceğiz.
    for (auto& task : tasks) {
        // Görevin saat ve dakika bilgisini al.
        int hour = task.getHour();
        int minute = task.getMinute();

        // Her gün tekrar edecek şekilde alarmı kur.
        uint8_t newAlarmId = Alarm.alarmRepeat(hour, minute, 0, alarmEventCallback);

        // Alarmın başarıyla kurulup kurulmadığını kontrol et.
        if (newAlarmId == dtINVALID_ALARM_ID) {
            // Başarısız olursa seri port'a hata mesajı yazdır.
            Serial.printf("HATA: '%s' ID'li gorev icin alarm yeniden olusturulamadi.\n", task.getId().c_str());
        } else {
            // Başarılı olursa, yeni alarm ID'sini görev nesnesine ata.
            task.setAlarmId(newAlarmId);
            // Bilgilendirme mesajı yazdır.
            Serial.printf("OK: '%s' ID'li gorev icin %s saatine %d ID'li alarm kuruldu.\n",
                          task.getId().c_str(),
                          task.getTime().c_str(),
                          newAlarmId);
        }
    }
}

TaskService::TaskService(StorageService& storageService) : storage(storageService) {
    // Constructor'ın gövdesi boştur, çünkü tüm gerekli atama işlemi
    // yukarıdaki başlatma listesinde zaten yapılmıştır.
}

void TaskService::deleteTask(String taskId)
{
    for (auto it = tasks.begin(); it != tasks.end(); ++it)
    {
        if (it->getId() == taskId)
        { // Task ID'si eşleşiyorsa
            uint8_t alarmId = it->getAlarmId();
            if (alarmId != dtINVALID_ALARM_ID && Alarm.isAllocated(alarmId))
            {
                Alarm.free(alarmId);
                Serial.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
                SerialBT.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
            }

            tasks.erase(it);
            Serial.printf("Task silindi: ID=%ld\n", taskId);
            SerialBT.printf("Task silindi: ID=%ld\n", taskId);

            return;
        }
    }

    Serial.println("Task bulunamadı.");
}

// Delete all tasks
void TaskService::deleteAllTasks()
{
    for (auto &task : tasks)
    {
        uint8_t alarmId = task.getAlarmId();
        if (alarmId != dtINVALID_ALARM_ID && Alarm.isAllocated(alarmId))
        {
            Alarm.free(alarmId);
            Serial.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
            SerialBT.printf("Alarm ID %d serbest bırakıldı\n", alarmId);
        }
    }

    tasks.clear();
    Serial.println("Tüm görevler silindi.");
    SerialBT.println("Tüm görevler silindi.");
}

std::vector<Task> TaskService::getTasks() const
{
    return tasks;
}

void TaskService::addTask(Task &task, void (*alarmEventCallback)())
{
    // Check if the task hour and minute are valid
    int hour = task.getHour();
    int minute = task.getMinute();

    char m[50];
    sprintf(m, "New alarm at %s with id %d", task.getTime().c_str(), task.getAlarmId());
    Serial.println(m);
    SerialBT.println(m);

    
    uint8_t alarmId = Alarm.alarmRepeat(hour, minute, 0, alarmEventCallback);

    if (alarmId == dtINVALID_ALARM_ID)
    {
        Serial.println("Failed to set alarm.");
        SerialBT.println("Failed to set alarm.");
        task.setAlarmId(alarmId);
    }
    else
    {
        task.setAlarmId(alarmId);

        char message[50];
        sprintf(message, "New alarm set at %s with id %d", task.getTime().c_str(), task.getAlarmId());
        Serial.println(message);
        SerialBT.println(message);

        tasks.push_back(task);

        storage.saveTasks(tasks);

        String taksStrr = storage.getTasks();

        Serial.println(taksStrr);

        Serial.println( "Alarms sync to esp");
        SerialBT.println( "Alarms sync to esp");
    }
}