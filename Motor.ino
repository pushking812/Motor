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

const byte STOP_SPD = 0;    // скорость остановки моторов (0-100)
const byte REVDIR_SPD = 25; // скорость при смене направления движения

const byte MLF_K = 1;      // корректировка скоростей вращения моторов
const byte MLR_K = 1;
const byte MRF_K = 1;
const byte MRR_K = 1;

const char DIR = 'D';      // код команды задания направления движения шасси
const byte DIR_FORW = 1;   // допустимое значение 'вперед'
const byte DIR_BACK = 2;   // допустимое значение 'назад'

// массив с допустимыми значениями направлений движения
int validDirValues[] = { DIR_FORW, DIR_BACK };

// разделил ANG на LFT и RGT
const char LFT = 'L';      // код команды задания угла поворота шасси влево
const byte LFT_MIN = 0;  // минимальное допустимое значение влево
const byte LFT_MAX = 45;   // максимальное допустимое значение влево

const char RGT = 'R';      // код команды задания угла поворота шасси вправо
const byte RGT_MIN = 0;  // минимальное допустимое значение вправо
const byte RGT_MAX = 45;   // максимальное допустимое значение вправо

// таймаут приема команды по последовательному порту
const unsigned int CMD_TIMEOUT = 100;

// Структура для хранения полученной команды
struct parsedCmd {
  char code;        // код команды
  int value;        // значение команды
};

// структура для хранения информации о команде
struct Commands {
  char code;           // код команды
  int minValue;        // минимальное допустимое значение команды
  int maxValue;        // максимальное допустимое значение команды
  int* validValues;    // массив допустимых значений
  int numValidValues;  // количество допустимых значений
};

// количество команд в массиве
const int NUM_COMMANDS = 4;

// массив команд
struct Commands commands[NUM_COMMANDS] = {
  { SPD, SPD_MIN, SPD_MAX, NULL, 0 },
  { DIR, DIR_BACK, DIR_FORW, validDirValues, 2 },
  { LFT, LFT_MIN, LFT_MAX, NULL, 0 },
  { RGT, RGT_MIN, RGT_MAX, NULL, 0 }
};

// флаги проверки значения команды в ParseCmd -> parseCmdValue
const byte IS_NUMBER = 0x01;
const byte IS_WORD = 0x02;

// максимальное количество символов в коде команды кода
const byte MAXCODLEN = 1;

// максимальная длина команды
const byte MAXCMDLEN = 16;

//---------------------------------------------------------------

// Функция void isChanged(), функция isChanged обеспечивает реакцию на
// изменения значений глобальных переменных Speed, Direction и Angle
void isChanged() {
  if (Speed != prevSpeed) {  // значение скорости изменилось
    prevSpeed = Speed;
    setSpeed();              // вызываем обработчик изменения скорости
    Serial.print("Speed: ");  Serial.print(Speed); Serial.print(" ");
  }
  if (Direction != prevDirection) {  // значение направления изменилось
    prevDirection = Direction;
    setDirection();                  // вызываем обработчик изменения направления
    Serial.print("Direction: "); Serial.print(Direction); Serial.print(" ");
  }
  if (Angle != prevAngle) {  // значение угла поворота изменилось
    prevAngle = Angle;
    setAngle();              // вызываем обработчик изменения угла поворота
    Serial.print("Angle: "); Serial.print(Angle);
  }
  Serial.println();
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

  setSpeed();
  setAngle();
  setDirection();
}

void loop() {
  // setCmd получает и обрабатывает команды последовательного порта,
  // задающие значения Speed, Direction, Angle;
  // при получении валидной команды:
  // сохраняет переменные Speed, Direction, Angle в prevSpeed, prevDirection или prevAngle
  // соответственно, инициализирует значения переменных Speed, Direction или Angle
  // получеными по последовательному порту значениями
  if (Serial.available() && setCmd()!=-1) isChanged();

  delay(100);
}
