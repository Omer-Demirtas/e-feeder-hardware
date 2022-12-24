#include "Map.hpp";
#include "Task.h";
#include "DB.hpp";

Map<String, int> m(10);
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
   for(int i = 0; i < 1; i++)
    {
      Serial.print(String(tasks[i].getHour()));
      Serial.print("*");
      Serial.print(tasks[i].getMinute());
      Serial.print("*");
      Serial.print(tasks[i].getSecond());
      Serial.println();
    }
   db.printStatus();
   
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
