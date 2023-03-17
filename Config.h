#pragma once

#include "Arduino.h"

// объявление переменных, констант, массивов
extern const byte MLF;  // порядковый номер левого переднего мотора mLF
extern const byte MLR;  // порядковый номер левого заднего мотора mLR
extern const byte MRF;  // порядковый номер правого переднего мотора mRF
extern const byte MRR;  // порядковый номер правого заднего мотора mRR

const byte COUNT = 4;   // количество моторов
const byte PINCOUNT = 2;   // количество пинов управления направлением вращения

extern const int speedPins[COUNT];  // номера пинов для регулировки скорости моторов
extern int speed[COUNT];      // значения скоростей моторов

extern const int directionPins[COUNT][PINCOUNT]; // номера пинов управления направлениями вращения моторов

extern int direction[COUNT][PINCOUNT];// направления вращения для каждого мотора, инициализируются 1

extern int Speed;          // текущая скорость шасси
extern int Direction;      // текущее направление движения шасси
extern int Angle;          // текущий угол поворота шасси
extern int prevSpeed;      // предыдущее значение скорости шасси
extern int prevDirection;  // предыдущее значение направления движения шасси
extern int prevAngle;      // предыдущее значение угла поворота шасси

extern const unsigned char PWM_MIN;
extern const unsigned char PWM_MAX;

extern char SPD;      // код команды задания скорости движения шасси
extern const byte SPD_MIN;    // минимальное допустимое значение
extern const byte SPD_MAX;  // максимальное допустимое значение

extern float KL;  // коэффициент скорости левых колес при повороте
extern float KR;  // коэффициент скорости правых колес при повороте

extern const byte STOP_SPD;     // скорость остановки моторов (0-100)
extern const byte REVDIR_SPD;  // скорость при смене направления движения

extern const float MLF_K;  // корректировка скоростей вращения моторов
extern const float MLR_K;
extern const float MRF_K;
extern const float MRR_K;

const char DIR = 'D';                                     // код команды задания направления движения шасси
const byte DIR_STOP = 0;                                  // допустимое значение 'останов'
const byte DIR_FORW = 1;                                  // допустимое значение 'вперед'
const byte DIR_BACK = 2;                                  // допустимое значение 'назад'
extern int validDirValues[3];    // допустимые значениями направлений движения

// разделил ANG на LFT и RGT
const char LFT = 'L';     // код команды задания угла поворота шасси влево
const char RGT = 'R';     // код команды задания угла поворота шасси вправо
const byte ANG_MIN = 0;   // минимальное допустимое значение угла
const byte ANG_MAX = 45;  // максимальное допустимое значение угла

// таймаут приема команды по последовательному порту, ms
const unsigned int CMD_TIMEOUT = 250;

// максимальное количество символов в коде команды кода
const byte MAXCODLEN = 1;

// максимальная длина команды
const byte MAXCMDLEN = 16;