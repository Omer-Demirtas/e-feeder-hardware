#include <Time.h>
#include <TimeAlarms.h>
#include "Map.hpp";
#include "Task.h";
#include "DB.hpp";
#include "Engine.h";
#include <AccelStepper.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

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
bool motorRun = false;
long lastMotorRun = 0;

/*
 * TODO: 
  * Create a project object and all of the function should be in that class.  
  * create a DB interface to abstract db operation
  * create a schedular service to manage tasks
  * Saving card logs online
*/

DB db("https://feeder-604ff-default-rtdb.firebaseio.com/");

void initCard()
{
  Serial.begin(115200);
  
  WiFi.begin("ASDFG123", "ASDFG123");

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("connected..! ");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  
  Serial.println("Initialize Completed Successfuly!");
}

/*
  Get the task list from web
*/
void initTasks()
{ 
   Task* tasks = db.getTasks();
   Serial.println("init Taks Started" + String(tasks[0].getHour()));
   //engine.run();
   
   for(int i = 0; i < db.getTaskSize(); i++)
   {
    Serial.println("task " + String(i) + " " + String(tasks[i].getHour()) + ":" + String(tasks[i].getMinute()));
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
  //stepper.moveTo(-10000000);
  //stepper.run();
  motorRun = true;
  lastMotorRun = millis();
}


void setup()
{
  setTime(7,29,40,1,1,10); // set time to 7:29:40am Jan 1 2010 
  
  //Alarm.alarmOnce(7,30,0, alarmEvent);
  //Alarm.alarmOnce(7,31,30, alarmEvent);

  initCard();
  db.initDB();
  initTasks();

  //pingTaskId = Alarm.timerRepeat(5, sendLivePing);

  stepper.setMaxSpeed(500);
  stepper.setAcceleration(100);
}

void loop() 
{
  /*
  digitalClockDisplay();
  Alarm.delay(7000);
  */
  /*
  if (stepper.distanceToGo() == 0)
    stepper.moveTo(-stepper.currentPosition());
   
  stepper.moveTo(-1000000);
  stepper.run();
   */ 
  long CurrentMillis = millis();
    
  if(CurrentMillis-lastMotorRun>=30000 && motorRun)
  { 
    Serial.println("10 second left");
     motorRun = false;
  }
  
  if(motorRun)
  {
    stepper.moveTo(-1000000);
    stepper.run();
    
  }else
  {
    digitalClockDisplay();
    Alarm.delay(1000);
  }
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
