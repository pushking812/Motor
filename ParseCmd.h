#pragma once

#include "Arduino.h"
#include "Debug.h"
#include "Validate.h"

// флаги проверки значения команды в ParseCmd -> parseCmdValue
const byte IS_NUMBER = 0x01;
const byte IS_WORD = 0x02;

// Получение команды из последовательного порта (setCmd->readStringUntil), 
// парсинг поступившей команды (setCmd->parseCmd),
// валидация обработанной команды (setCmd->isValidCommand),
// изменение значений глобальных переменных Speed, Direction и Angle
int setCmd(char* cmd);
//int setCmd();

// Получение команды из последовательного порта 
char* readStringUntil(char terminator, unsigned int timeout);

// Парсинг поступившей команды
parsedCmd* parseCmd(const char* cmd);

// Парсинг кода команды
char parseCmdCode(const char* cmd);

// Парсинг параметра команды
int parseCmdValue(const char* cmd, byte flags);

// Изменение значений соответствующих переменных
// Speed, Direction или Angle
void runCmd(parsedCmd* p);
