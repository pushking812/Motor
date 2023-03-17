/* Программма управления движением колесного шасси при помощи микроконтроллера Arduino Nano*/

#include "Debug.h"
#include "Config.h"
#include "Validate.h"

// флаги проверки значения команды в ParseCmd -> parseCmdValue
const byte IS_NUMBER;
const byte IS_WORD;

// Функция void isChanged(), функция isChanged обеспечивает реакцию на
// изменения значений глобальных переменных Speed, Direction и Angle
void isChanged() {
  const char* DBG_FUNC="isChanged";

  if (Speed != prevSpeed) {  // значение скорости изменилось
    DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "prevSpeed", prevSpeed, false);
    DebugMsg(DBG_PRE_INF, "", "", "Speed", Speed, true);
    prevSpeed = Speed;
    setSpeed();  // вызываем обработчик изменения скорости
  }
  if (Direction != prevDirection) {  // значение направления изменилось
    int savedDirection = Direction;
    Direction = DIR_STOP;  // Остановка всех моторов
    setDirection();
    delay(1000);

    DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "prevDirection", prevDirection, false);
    DebugMsg(DBG_PRE_INF, "", "", "Direction", Direction, true);
    Direction = savedDirection;
    prevDirection = Direction;
    setDirection();  // вызываем обработчик изменения направления
  }
  if (Angle != prevAngle) {  // значение угла поворота изменилось
    DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "prevAngle", prevAngle, false);
    DebugMsg(DBG_PRE_INF, "", "", "Angle", Angle, true);
    prevAngle = Angle;
    setAngle();  // вызываем обработчик изменения угла поворота
  }
}

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < COUNT; i++) {
    // установка пинов для управления скоростью моторов в режим OUTPUT
    pinMode(speedPins[i], OUTPUT);
    // установка значений LOW на всех пинах для управления скоростью моторов
    digitalWrite(speedPins[i], LOW);
  }

  for (int i = 0; i < COUNT; i++) {
    for (int j = 0; j < PINCOUNT; j++) {
      // установка пинов для управления направлением вращения моторов в режим OUTPUT
      pinMode(directionPins[i][j], OUTPUT);
      // установка значений на всех пинах для управления направлением вращения моторов
      digitalWrite(directionPins[i][j], LOW);
    }
  }


  
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


