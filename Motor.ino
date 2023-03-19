// Программма управления движением четырехколесного шасси 
// с использованием платы Arduino Nano

#include "ParseCmd.h"
#include "Chassis.h"
//#include "TimerOne.h"

void setup() {
  Serial.begin(9600);

  //Timer1.initialize(10000);  // установить частоту 100 Гц (10 мс)
  //Timer1.attachInterrupt(isChanged);  // установить функцию-обработчик прерывания

  // установка пинов моторов
  initMotorPins();

  //isChanged();
}

int i=0;
char* cmds[7] ={"D,1", "S,75", "L,45", "R,0", "S,76", "R,44", "S,0"};
int executionInterval[8] ={100, 1000, 2000, 5000, 1000, 2000, 5000, 500};

unsigned long lastExecutionTime = 0;

void loop() {
  // setCmd получает и обрабатывает команды последовательного порта,
  // задающие значения Speed, Direction, Angle;
  // при получении валидной команды:
  // сохраняет переменные Speed, Direction, Angle в prevSpeed, prevDirection или prevAngle
  // соответственно, инициализирует значения переменных Speed, Direction или Angle
  // получеными по последовательному порту значениями

  //if (Serial.available() && setCmd() != -1);

  unsigned long now = millis();
  if (now - lastExecutionTime >= executionInterval[i]) {
    if (i < 7) {
      setCmd(cmds[i]);
      const char* DBG_FUNC = "loop";
      DebugMsg(DBG_PRE_INF, DBG_FUNC, "****command manual****", "cmds[i]", cmds[i], false);
      DebugMsg(DBG_PRE_INF, "", "", "executionInterval[i]", executionInterval[i], false);
      DebugMsg(DBG_PRE_INF, "", "", "i", i, true);
      i++;
    }
    lastExecutionTime = now;
  }

  isChanged();
  
}


