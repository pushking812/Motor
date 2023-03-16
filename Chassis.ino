// Функция setSpeed задает скорость движения шасси
void setSpeed() {
  const char* DBG_FUNC="setSpeed";
  // Установка значения Angle равным 0
  // Angle = 0;

  // Установка скорости вращения для каждого мотора
  if (Speed == SPD_MIN) Direction = DIR_STOP;

  float leftSpeed = Speed * KL;
  float rightSpeed = Speed * KR;

  DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "prevSpeed", prevSpeed, false);
  DebugMsg(DBG_PRE_INF, "", "", "Speed", Speed, false);
  DebugMsg(DBG_PRE_INF, "", "", "leftSpeed", leftSpeed, false);
  DebugMsg(DBG_PRE_INF, "", "", "rightSpeed", rightSpeed, false);
  DebugMsg(DBG_PRE_INF, "", "", "prevDirection", prevDirection, false);
  DebugMsg(DBG_PRE_INF, "", "", "Direction", Direction, true);

  setMotorSpeed(MLF, leftSpeed * MLF_K);
  setMotorSpeed(MLR, leftSpeed * MLR_K);
  setMotorSpeed(MRF, rightSpeed * MRF_K);
  setMotorSpeed(MRR, rightSpeed * MRR_K);
}

// Функция setAngle задает угол поворота шасси
void setAngle() {
  const char* DBG_FUNC="setAng";
  static char i = 1;
  // Вычисление коэффициента k, определяющего отношние скоростей моторов при повороте
  float k = (45 - abs(Angle)) / 45.0;

  // Установка скорости вращения для каждого мотора
  float leftSpeed, rightSpeed;
  
  char sign = 1;
  
  if (Angle < 0) {  // Поворот влево
    leftSpeed = Speed / (1 - k) - Speed;
    rightSpeed = Speed;
    sign=-1;
  } else if (Angle > 0) {  // Поворот вправо
    leftSpeed = Speed;
    rightSpeed = Speed / (1 - k) - Speed;
  } else {  // Движение прямо
    leftSpeed = Speed;
    rightSpeed = Speed;
  }

  KL = leftSpeed/100;  KR = rightSpeed/100; 

  if (leftSpeed == rightSpeed) {
    KL = 1.0;  KR = 1.0;    
  }
   
  byte SpeedMap = map(Angle*sign, ANG_MIN, ANG_MAX, SPD_MAX, 50);

  if (Speed > 0) i *= -1;
  Speed = constrain(SpeedMap, SPD_MIN, Speed) + i;

  DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "prevAngle", prevAngle, false);
  DebugMsg(DBG_PRE_INF, "", "", "Angle", Angle, false);
  DebugMsg(DBG_PRE_INF, "", "", "k", k, false);
  DebugMsg(DBG_PRE_INF, "", "", "leftSpeed", leftSpeed, false);
  DebugMsg(DBG_PRE_INF, "", "", "rightSpeed", rightSpeed, false);
  DebugMsg(DBG_PRE_INF, "", "", "SpeedMap", SpeedMap, false);
  DebugMsg(DBG_PRE_INF, "", "", "prevSpeed", prevSpeed, false);
  DebugMsg(DBG_PRE_INF, "", "", "Speed", Speed, true);
}

// Функция setDirection задает направление движения шасси
void setDirection() {
  const char* DBG_FUNC="setDirection";
  // Установка направления вращения для моторов на левой и правой сторонах шасси

  if (Direction == DIR_FORW) {  // движение вперед
    setAllMotorDirection(DIR_FORW);
  } else if (Direction == DIR_BACK) {  // движение назад
    setAllMotorDirection(DIR_BACK);
  } else if (Direction == DIR_STOP) {  // стоп машина
    Speed = STOP_SPD;

    DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "Direction", Direction, false);
    DebugMsg(DBG_PRE_INF, "", "", "Speed", Speed, true);

    setAllMotorDirection(DIR_STOP);

    return;
  }

  // Установка скорости вращения для каждого мотора, с коэффициентами корректировок
  Speed = REVDIR_SPD;
  
  DebugMsg(DBG_PRE_INF, DBG_FUNC, DBG_MSG_VAL, "Direction", Direction, false);
  DebugMsg(DBG_PRE_INF, "", "", "Speed", Speed, true);
}

void setAllMotorDirection(byte dir) {
  setMotorDirection(MLF, dir);
  setMotorDirection(MLR, dir);
  setMotorDirection(MRF, dir);
  setMotorDirection(MRR, dir);
}
