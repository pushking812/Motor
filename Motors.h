#pragma once

#include "Arduino.h"
#include "Debug.h"
#include "Config.h"

const byte COUNT = 4;   // количество моторов
const byte PINCOUNT = 2;   // количество пинов управления направлением вращения

// объявление переменных, констант, массивов
extern const int speedPins[COUNT];

// текущие значения скоростей вращения моторов
extern int speed[COUNT];

// пины управления направлением вращения моторов
extern const int directionPins[COUNT][PINCOUNT];

// текущие направления вращения моторов
extern int direction[COUNT][PINCOUNT];

// установка пинов моторов
void initMotorPins();

// Устанавливает направление вращения мотора
void setMotorDirection(int motor, int dir);

// Устанавливает скорость вращения мотора
void setMotorSpeed(int motor, int spd);
