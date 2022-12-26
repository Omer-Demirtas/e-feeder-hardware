#include <Time.h>
#include <TimeAlarms.h>
#include "Map.hpp";
#include "Task.h";
#include "DB.hpp";
#include "Engine.h";

AlarmID_t myAlarm = 0;

Engine engine(8, 10, 9, 11);

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

ISR(TIMER1_COMPA_vect)
{
  Serial.println("Timer");
}

void setup()
{
  setTime(7,29,55,1,1,10); // set time to 7:29:40am Jan 1 2010 

  initCard();
  db.initDB();
  initTasks();

  pingTaskId = Alarm.timerRepeat(5, sendLivePing);

    cli();
  /* Ayarlamaların yapılabilmesi için öncelikle kesmeler durduruldu */

  /* Timer1 kesmesi saniyede bir çalışacak şekilde ayarlanacaktır (1 Hz)*/
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 15624;
  /* Bir saniye aralıklar için zaman sayıcısı ayarlandı */
  TCCR1B |= (1 << WGM12);
  /* Adımlar arasında geçen süre kristal hızının 1024'e bölümü olarak ayarlandı */
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  /* Timer1 kesmesi aktif hale getirildi */

  sei();

  
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
