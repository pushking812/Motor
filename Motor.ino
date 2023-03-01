/* Программма управления движением колесного шасси при помощи микроконтроллера Arduino Nano*/

// объявление переменных, констант, массивов
const byte MLF = 1;  // порядковый номер левого переднего мотора mLF
const byte MLR = 2;  // порядковый номер левого заднего мотора mLR
const byte MRF = 3;  // порядковый номер правого переднего мотора mRF
const byte MRR = 4;  // порядковый номер правого заднего мотора mRR

const byte COUNT = 4;     // количество моторов
const byte PINCOUNT = 2;  // количество пинов управления направлением вращения

int speedPins[COUNT] = { 3, 5, 6, 9 };  // номера пинов для регулировки скорости моторов
int speed[COUNT] = { 0, 0, 0, 0 };      // значения скоростей моторов

// номера пинов управления направлениями вращения моторов
int directionPins[COUNT][PINCOUNT] = {
  { 2, 4 },    // направление для мотора mLF
  { 7, 8 },    // направление для мотора mLR
  { 10, 11 },  // направление для мотора mRF
  { 12, 13 }   // направление для мотора mRR
};

// направления вращения для каждого мотора, инициализируются 1
int direction[COUNT][PINCOUNT] = {
  { 1, 1 },  // направления для мотора mLF
  { 1, 1 },  // направления для мотора mLR
  { 1, 1 },  // направления для мотора mRF
  { 1, 1 }   // направления для мотора mRR
};

int Speed = 0;          // текущая скорость шасси
int Direction = 1;      // текущее направление движения шасси
int Angle = 0;          // текущий угол поворота шасси
int prevSpeed = 0;      // предыдущее значение скорости шасси
int prevDirection = 1;  // предыдущее значение направления движения шасси
int prevAngle = 0;      // предыдущее значение угла поворота шасси

const unsigned char PWM_MIN = 0;
const unsigned char PWM_MAX = 255;

const char SPD = 'S';      // код команды задания скорости движения шасси
const byte SPD_MIN = 0;    // минимальное допустимое значение
const byte SPD_MAX = 100;  // максимальное допустимое значение

const char DIR = 'D';      // код команды задания направления движения шасси
const byte DIR_FORW = 1;   // допустимое значение 'вперед'
const byte DIR_BACK = -1;  // допустимое значение 'назад'

// массив с допустимыми значениями направлений движения
int validDirValues[] = { DIR_FORW, DIR_BACK };

const char ANG = 'A';      // код команды задания угла поворота шасси
const byte ANG_MIN = -45;  // минимальное допустимое значение
const byte ANG_MAX = 45;   // максимальное допустимое значение

// структура для хранения информации о команде
struct Commands {
  char code;           // код команды
  int minValue;        // минимальное допустимое значение команды
  int maxValue;        // максимальное допустимое значение команды
  int* validValues;    // массив допустимых значений
  int numValidValues;  // количество допустимых значений
};

// массив команд
struct Commands commands[3] = { { SPD, SPD_MIN, SPD_MAX, NULL, 0 },
                                { DIR, DIR_BACK, DIR_FORW, validDirValues, 2 },
                                { ANG, ANG_MIN, ANG_MAX, NULL, 0 } };
// количество команд в массиве
const int NUM_COMMANDS = 3;

void setup() {
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

void loop() {
  // parseCmd получает и обрабатывает команды последовательного порта,
  // задающие значения Speed, Direction, Angle;
  // при получении валидной команды:
  // сохраняет переменные Speed, Direction, Angle в prevSpeed, prevDirection или prevAngle
  // соответственно, инициализирует значения переменных Speed, Direction или Angle
  // получеными по последовательному порту значениями
  parseCmd();

  delay(100);
}

//---------------------------------------------------------------

// Функция для парсинга кода команды из строки
char parseCmdCode(const char* cmd) {
  return cmd[0];
}

// Функция для парсинга значения команды из строки
int parseCmdValue(const char* cmd) {
  char* value = strtok(NULL, ",");
  if (value == NULL) return -1;
  return atoi(value);
}

// Функция для проверки валидности значения команды
bool isValidValue(Commands& cmd, int val) {
  if (cmd.validValues != NULL && cmd.numValidValues > 0) {
    // проверка по массиву допустимых значений
    for (int j = 0; j < cmd.numValidValues; j++) {
      if (val == cmd.validValues[j]) return true;
    }
  } else {
    // проверка по минимальному и максимальному значению
    if (val >= cmd.minValue && val <= cmd.maxValue) return true;
  }
  return false;
}

// Функция parseCmd получает и обрабатывает команды последовательного порта,
// задает значениях глобальных переменных Speed, Direction и Angle
void parseCmd() {
  char* cmd = Serial.readStringUntil('\n').c_str();
  char code = parseCmdCode(cmd);
  int value = parseCmdValue(cmd);

  // искать команду в массиве и вызвать ее обработчик
  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (code == commands[i].code) {
      if (isValidValue(commands[i], value)) isChanged(code);
      else Serial.println("Error: Invalid value");

      return;
    }
  }

  // команда не найдена
  Serial.println("Error: Invalid command code");
}

//---------------------------------------------------------------

// Функция void isChanged(), функция isChanged обеспечивает реакцию на
// изменения значений глобальных переменных Speed, Direction и Angle
void isChanged(char cmd) {
  switch (cmd) {
    case SPD:
      if (Speed != prevSpeed) {  // значение скорости изменилось
        setSpeed();              // вызываем обработчик изменения скорости
      }
      break;
    case DIR:
      if (Direction != prevDirection) {  // значение направления изменилось
        setDirection();                  // вызываем обработчик изменения направления
      }
      break;
    case ANG:
      if (Angle != prevAngle) {  // значение угла поворота изменилось
        setAngle();              // вызываем обработчик изменения угла поворота
      }
      break;
    default:
      break;
  }
}
//---------------------------------------------------------------
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
//---------------------------------------------------------------
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
//---------------------------------------------------------------
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
//---------------------------------------------------------------
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