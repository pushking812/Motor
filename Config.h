#pragma once

#include "Arduino.h"

const byte COUNT = 4;   // количество моторов
const byte PINCOUNT = 2;   // количество пинов управления направлением вращения

// Обозначения моторов:
// левый передний - mLF, левый задний - mLR
// правый передний - mRF, правый задний - mRR

// пины управления скоростью вращения моторов
const byte SP_MLF = 3, SP_MLR = 5, SP_MRF = 6, SP_MRR = 9;

// пины управления направлением вращения моторов
const byte DP_MLF1 = 2, DP_MLR1 = 7, DP_MRF1 = 10, DP_MRR1 = 12;
const byte DP_MLF2 = 4, DP_MLR2 = 8, DP_MRF2 = 11, DP_MRR2 = 13;

extern int Speed;          // текущая скорость шасси
extern int Direction;      // текущее направление движения шасси
extern int Angle;          // текущий угол поворота шасси
extern int prevSpeed;      // предыдущее значение скорости шасси
extern int prevDirection;  // предыдущее значение направления движения шасси
extern int prevAngle;      // предыдущее значение угла поворота шасси

const unsigned char PWM_MIN = 0;
const unsigned char PWM_MAX = 255;

// команда задающая скорость движения шасси
const char SPD = 'S';           // код команды 
const byte SPD_MIN = 0;   // минимальное значение
const byte SPD_MAX = 100; // максимальное значение

const byte STOP_SPD = 0;     // скорость остановки моторов (0-100)
const byte REVDIR_SPD = 25;  // скорость при смене направления движения

// "ручная" корректировка скоростей вращения моторов
const float MLF_K = 1, MLR_K = 1, MRF_K = 1, MRR_K = 1;

const char DIR = 'D';                                     // код команды задания направления движения шасси
const byte DIR_STOP = 0;                                  // допустимое значение 'останов'
const byte DIR_FORW = 1;                                  // допустимое значение 'вперед'
const byte DIR_BACK = 2;                                  // допустимое значение 'назад'

// разделил ANG на LFT и RGT
const char LFT = 'L';     // код команды задания угла поворота шасси влево
const char RGT = 'R';     // код команды задания угла поворота шасси вправо
const byte ANG_MIN = 0;   // минимальное допустимое значение угла
const byte ANG_MAX = 45;  // максимальное допустимое значение угла
