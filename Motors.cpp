#include "Motors.h"

// Обозначения моторов:
// левый передний - mLF, левый задний - mLR
// правый передний - mRF, правый задний - mRR

// объявление переменных, констант, массивов
const int speedPins[COUNT] = { SP_MLF, SP_MLR, SP_MRF, SP_MRR };  

// текущие значения скоростей вращения моторов
int speed[COUNT] = { 0, 0, 0, 0 };      // значения скоростей моторов

// пины управления направлением вращения моторов
const int directionPins[COUNT][PINCOUNT] = {
  { DP_MLF1, DP_MLF2 }, 
  { DP_MLR1, DP_MLR2 }, 
  { DP_MRF1, DP_MRF2 }, 
  { DP_MRR1, DP_MRR2 }   
};

// текущие направления вращения моторов
int direction[COUNT][PINCOUNT] = { { 1, 1 }, { 1, 1 }, { 1, 1 }, { 1, 1 } };

// установка пинов моторов
void initMotorPins() {
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
}

// Функция setMotorDirection устанавливает направление вращения мотора
void setMotorDirection(int motor, int dir) {
  const char* DBG_FUNC="setMotorDirection";
  
  if (dir == DIR_FORW) {  // установка направления вперед
    digitalWrite(directionPins[motor - 1][0], LOW);
    digitalWrite(directionPins[motor - 1][1], HIGH);
  } else if (dir == DIR_BACK) {  // установка направления назад
    digitalWrite(directionPins[motor - 1][0], HIGH);
    digitalWrite(directionPins[motor - 1][1], LOW);
  } else if (dir == DIR_STOP) {  // установка направления назад
    digitalWrite(directionPins[motor - 1][0], HIGH);
    digitalWrite(directionPins[motor - 1][1], HIGH);
  } else {  // неверное направление
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_INVVAL, "dir", dir, true);
    return;
  }

  DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "motor", motor, false);
  DebugMsg(DBG_PRE_INF, "", "", "dir", dir, true);

  // сохранение направления вращения мотора
  direction[motor - 1][0] = digitalRead(directionPins[motor - 1][0]);
  direction[motor - 1][1] = digitalRead(directionPins[motor - 1][1]);
}

// Функция setMotorSpeed устанавливает скорость вращения мотора
void setMotorSpeed(int motor, int spd) {
  const char* DBG_FUNC="setMotorSpeed";  
  // Преобразование заданной скорости в диапазон ШИМ-сигнала (0-255)
  int pwm = map(spd, SPD_MIN, SPD_MAX, PWM_MIN, PWM_MAX);

  // Проверка на допустимость значений параметров
  if (motor < 1 || motor > COUNT) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_INVVAL, "motor", motor, true);
    return;
  }
  if (spd < SPD_MIN || spd > SPD_MAX) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_INVVAL, "spd", spd, true);
    return;
  }

  DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "motor", motor, false);
  DebugMsg(DBG_PRE_INF, "", "", "spd", spd, true);

  // Сохранение заданной скорости в массиве speed[][]
  speed[motor - 1] = spd;

  // Установка требуемой скорости на пин speedPins[][]
  analogWrite(speedPins[motor - 1], pwm);
}
