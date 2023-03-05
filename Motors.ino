// Функция setMotorDirection устанавливает направление вращения мотора
void setMotorDirection(int motor, int dir) {
  if (dir == 1) {  // установка направления вперед
    digitalWrite(directionPins[motor][0], LOW);
    digitalWrite(directionPins[motor][1], HIGH);
  } else if (dir == -1) {  // установка направления назад
    digitalWrite(directionPins[motor][0], HIGH);
    digitalWrite(directionPins[motor][1], LOW);
  } else {  // неверное направление
    Serial.println("Error: Invalid motor direction");
    return;
  }

  // сохранение направления вращения мотора
  direction[motor][0] = digitalRead(directionPins[motor][0]);
  direction[motor][1] = digitalRead(directionPins[motor][1]);
}

// Функция setMotorSpeed устанавливает скорость вращения мотора
void setMotorSpeed(int motor, int spd) {
  // Преобразование заданной скорости в диапазон ШИМ-сигнала (0-255)
  int pwm = map(spd, SPD_MIN, SPD_MAX, PWM_MIN, PWM_MAX);

  // Проверка на допустимость значений параметров
  if (motor < 1 || motor > COUNT) {
    Serial.println("Error: Invalid motor number");
    return;
  }
  if (spd < SPD_MIN || spd > SPD_MAX) {
    Serial.println("Error: Invalid speed value");
    return;
  }

  // Сохранение заданной скорости в массиве speed[][]
  speed[motor - 1] = spd;

  // Установка требуемой скорости на пин speedPins[][]
  analogWrite(speedPins[motor - 1], pwm);
}
