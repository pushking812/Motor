
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
