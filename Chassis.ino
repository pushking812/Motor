// Функция setSpeed задает скорость движения шасси
void setSpeed() {
  // Установка значения Angle равным 0
  // Angle = 0;

  // Установка скорости вращения для каждого мотора
  if (Speed == SPD_MIN) {
    Direction = DIR_STOP;
  }

  int leftSpeed = Speed * KL;
  int rightSpeed = Speed * KR;

  setMotorSpeed(MLF, leftSpeed * MLF_K);
  setMotorSpeed(MLR, leftSpeed * MLR_K);
  setMotorSpeed(MRF, rightSpeed * MRF_K);
  setMotorSpeed(MRR, rightSpeed * MRR_K);
}

// Функция setAngle задает угол поворота шасси
void setAngle() {
  Serial.println("Info: setAngle");
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

  KL = leftSpeed;
  KR = rightSpeed;
  Serial.print("Info: setAngle leftSpeed, rightSpeed: ");
  Serial.print(leftSpeed);
  Serial.print(", ");
  Serial.println(rightSpeed);
  byte SpeedMap = map(Speed, ANG_MAX, ANG_MIN, SPD_MIN, SPD_MAX);
  Speed = constrain(SpeedMap, SPD_MIN, Speed);
  Serial.print("Info: setAngle setting speed: ");
  Serial.println(Speed);
}

// Функция setDirection задает направление движения шасси
void setDirection() {
  // Установка направления вращения для моторов на левой и правой сторонах шасси
  Serial.println("Info: setDirection");
  if (Direction == DIR_FORW) {  // движение вперед
    setAllMotorDirection(DIR_FORW);
  } else if (Direction == DIR_BACK) {  // движение назад
    setAllMotorDirection(DIR_BACK);
  } else if (Direction == DIR_STOP) {  // стоп машина
    Serial.println("Info: setDirection stopping motors");
    Speed = STOP_SPD;
    Serial.println("Info: setDirection setting zero direction");
    setAllMotorDirection(DIR_STOP);
    return;
  }

  // Установка скорости вращения для каждого мотора, с коэффициентами корректировок
  Serial.println("Info: setDirection setting speed: REVDIR_SPD");
  Speed = REVDIR_SPD;
}

void setAllMotorDirection(byte dir) {
  setMotorDirection(MLF, dir);
  setMotorDirection(MLR, dir);
  setMotorDirection(MRF, dir);
  setMotorDirection(MRR, dir);
}
