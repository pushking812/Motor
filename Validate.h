#pragma once

#include "Arduino.h"
#include "Config.h"

// Структура для хранения полученной команды
struct parsedCmd {
  char code;  // код команды
  int value;  // значение команды
};

// структура для хранения информации о команде
struct Command {
  char code;           // код команды
  int minValue;        // минимальное допустимое значение команды
  int maxValue;        // максимальное допустимое значение команды
  int* validValues;    // массив допустимых значений
  int numValidValues;  // количество допустимых значений
};

// количество команд в массиве
const int NUM_COMMANDS = 4;

// массив команд
typedef Command Commands[NUM_COMMANDS];

// таймаут приема команды по последовательному порту, ms
const unsigned int CMD_TIMEOUT = 250;

// максимальное количество символов в коде команды кода
const byte MAXCODLEN = 1;

// максимальная длина команды
const byte MAXCMDLEN = 16;

// Валидация команды
bool isValidCommand(parsedCmd* p);

// Валидация кода команды
bool isValidCmdCode(parsedCmd* p);

// Валидация параметра кода команды
bool isValidValue(parsedCmd* p);

// Проверка - положительная цифра
bool isDigit(char c);

// Проверка - символ латиницы
bool isAlpha(char c);

// Проверка - положительное число
int isNumber(const char* str);

// Проверка - слово из букв латинского алфавита
int isWord(const char* str);
