// Программма управления движением четырехколесного шасси 
// с использованием платы Arduino Nano

#include "ParseCmd.h"
#include "Chassis.h"

void setup() {
  Serial.begin(9600);

  // установка пинов моторов
  initMotorPins();

  //isChanged();
}

void loop() {
  // setCmd получает и обрабатывает команды последовательного порта,
  // задающие значения Speed, Direction, Angle;
  // при получении валидной команды:
  // сохраняет переменные Speed, Direction, Angle в prevSpeed, prevDirection или prevAngle
  // соответственно, инициализирует значения переменных Speed, Direction или Angle
  // получеными по последовательному порту значениями
  if (Serial.available() && setCmd() != -1);
  isChanged();

  delay(100);
}


