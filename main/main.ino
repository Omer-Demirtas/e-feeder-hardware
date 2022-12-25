#include <Time.h>
#include <TimeAlarms.h>
#include "Map.hpp";
#include "Task.h";
#include "DB.hpp";

AlarmID_t myAlarm = 0;
int pingTaskId = 0;

/*
 * TODO: 
  * Create a project object and all of the function should be in that class.  
  * create a DB interface to abstract db operation
  * create a schedular service to manage tasks
*/

DB db("asd");

void initCard()
{
  Serial.begin(9600);

  Serial.println("Card Started");
}

/*
  Get the task list from web
*/
void initTasks()
{ 
   Task* tasks = db.getTasks();
   
   for(int i = 0; i < db.getTaskSize(); i++)
   {
    scheduleTask(tasks[i], i, alarmEvent);
   }
}

/*
 * to control any changes for this device 
*/
void sendLivePing() 
{
  Serial.println("Live ping");
  
  if(db.sendLivePing())
  { 
    initTasks();
  }
}

void scheduleTask(Task task, int index, void (*func)())
{
  int ID = Alarm.alarmOnce(AlarmHMS(task.getHour(),task.getMinute(), 0), func);
  db.setTaskId(index, ID);
}

void alarmEvent()
{
  Serial.println("Alarm");
}

void setup()
{
  setTime(7,29,55,1,1,10); // set time to 7:29:40am Jan 1 2010 

  initCard();
  db.initDB();
  initTasks();

  pingTaskId = Alarm.timerRepeat(5, sendLivePing);
}

void loop() 
{
  digitalClockDisplay();
  Alarm.delay(7000);
}


void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.println();
}

void printDigits(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}
