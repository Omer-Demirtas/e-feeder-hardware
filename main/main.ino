#include <Time.h>
#include <TimeAlarms.h>
#include "Map.hpp";
#include "Task.h";
#include "DB.hpp";
#include "Engine.h";
#include <AccelStepper.h>

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution
AlarmID_t myAlarm = 0;


// ULN2003 Motor Driver Pins
#define IN1 5
#define IN2 4
#define IN3 14
#define IN4 12
//Engine engine(8, 10, 9, 11);
AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

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
  Serial.begin(115200);

  Serial.println("Card Started");
}

/*
  Get the task list from web
*/
void initTasks()
{ 
   Task* tasks = db.getTasks();
   
   //engine.run();
   
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
  delay(5000);
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

  stepper.setMaxSpeed(500);
  stepper.setAcceleration(100);
}

void loop() 
{
  /*
  digitalClockDisplay();
  Alarm.delay(7000);
  */
  stepper.moveTo(stepsPerRevolution);
  stepper.run();
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
