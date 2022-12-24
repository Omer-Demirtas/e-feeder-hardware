#include "Map.hpp";
#include "Task.h";
#include "DB.hpp";

Map<String, int> m(10);
Task task(1);
DB db("asd");

void initCard()
{
  Serial.begin(9600);

  Serial.println("Card Started");
}

/*
  Get the task list from web
*/
void initializeTasks()
{ 
   db.initDB();

   Task* tasks = db.getTasks();
   
   db.printStatus();
   Serial.println(tasks[0].getId());
}

/*
 * to control any changes for this device 
*/
void sendLivePing() { }

void setup()
{
  initCard();
  initializeTasks();
}

void loop() 
{

}
