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

// максимальная длина команды
const byte MAXCMDLEN=16;

// максимальное количество символов в коде команды кода
const byte MAXCODLEN=1;

// таймаут приема команды по последовательному порту
const unsigned int CMD_TIMEOUT=1000;

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

void setup() {
  Serial.begin(9600);
  //Serial.println("Info: setup() start");
  
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
  Serial.println("Info: setup() end");

  setSpeed();
  setAngle();
  setDirection();
}

void loop() {
  // Serial.println("Info: loop() start");
  // parseCmd получает и обрабатывает команды последовательного порта,
  // задающие значения Speed, Direction, Angle;
  // при получении валидной команды:
  // сохраняет переменные Speed, Direction, Angle в prevSpeed, prevDirection или prevAngle
  // соответственно, инициализирует значения переменных Speed, Direction или Angle
  // получеными по последовательному порту значениями
  parseCmd();

  delay(100);
  //Serial.println("Info: setup() end");
}
