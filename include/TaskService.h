#ifndef TASK_SERVICE_H
#define TASK_SERVICE_H

#include <ArduinoJson.h>

// TaskService sınıfı
class TaskService {
private:
    String apiUrl;                 // Görevleri çekmek için API URL'si
    int taskList[10];    // Yerel görev listesi

public:
    TaskService(const String& url); // Constructor
    void fetchTasks();              // Görevleri çekme
    //const tasklist getTasks() const; // Görev listesini alma
    void completeTask(int index);   // Görevi tamamlandı olarak işaretleme
};

#endif // TASK_SERVICE_H
