#include "Task.h";

Task::Task(int _hour, int _minute, int _second) {
  hour = _hour;
  minute = _minute;
  second = _second;
}

Task::Task(String _key, int _amount) {
  int h = _key.substring(0, 2).toInt();
  int m = _key.substring(3, 5).toInt();
  int s = _key.substring(6, 8).toInt();

  hour = h;
  minute = m;
  second = s;
  amount = _amount;
}
