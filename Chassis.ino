// Функция setSpeed задает скорость движения шасси
void setSpeed() {
  // Установка значения Angle равным 0
  Angle = 0;
  

  // Установка скорости вращения для каждого мотора
  int leftSpeed = Speed * 1;
  int rightSpeed = Speed;
  setMotorSpeed(MLF, leftSpeed);
  setMotorSpeed(MLR, leftSpeed);
  setMotorSpeed(MRF, rightSpeed);
  setMotorSpeed(MRR, rightSpeed);
}

// Функция setAngle задает угол поворота шасси
void setAngle() {
  // Вычисление коэффициента k, определяющего отношение скоростей моторов при повороте
  float k = (45 - abs(Angle)) / 45.0;

  // Установка скорости вращения для каждого мотора
  int leftSpeed, rightSpeed;
  if (Angle < 0) {  // Поворот влево
    leftSpeed = Speed * (1 - k);
    rightSpeed = Speed;
  } else if (Angle > 0) {  // Поворот вправо
    leftSpeed = Speed;
    rightSpeed = Speed * (1 - k);
  } else {  // Движение прямо
    leftSpeed = Speed;
    rightSpeed = Speed;
  }
  setMotorSpeed(MLF, leftSpeed);
  setMotorSpeed(MLR, leftSpeed);
  setMotorSpeed(MRF, rightSpeed);
  setMotorSpeed(MRR, rightSpeed);
}

// Функция setDirection задает направление движения шасси
void setDirection() {
  // Остановка всех моторов
  setMotorSpeed(MLF, 0);
  setMotorSpeed(MLR, 0);
  setMotorSpeed(MRF, 0);
  setMotorSpeed(MRR, 0);

  // Установка направления вращения для моторов на левой и правой сторонах шасси
  if (Direction == 1) {  // движение вперед
    setMotorDirection(MLF, DIR_FORW);
    setMotorDirection(MLR, DIR_FORW);
    setMotorDirection(MRF, DIR_FORW);
    setMotorDirection(MRR, DIR_FORW);
  } else if (Direction == -1) {  // движение назад
    setMotorDirection(MLF, DIR_BACK);
    setMotorDirection(MLR, DIR_BACK);
    setMotorDirection(MRF, DIR_BACK);
    setMotorDirection(MRR, DIR_BACK);
  }

  // Установка скорости вращения для каждого мотора
  int leftSpeed = Speed * 1;
  int rightSpeed = Speed;
  setMotorSpeed(MLF, leftSpeed);
  setMotorSpeed(MLR, leftSpeed);
  setMotorSpeed(MRF, rightSpeed);
  setMotorSpeed(MRR, rightSpeed);
}
