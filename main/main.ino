#include "Map.hpp";
#include "Task.h";
#include "DB.hpp";

/*
 * TODO: 
  * Create a project object and all of the function should be in that class.  
  * create a DB interface to abstract db operation
*/

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
      Serial.println(tasks[i].getTimeString());
    }
   
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
