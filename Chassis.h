#pragma once

#include "Arduino.h"
#include "Debug.h"
#include "Motors.h"
#include "Config.h"

// Обозначения моторов:
// левый передний - mLF, левый задний - mLR
// правый передний - mRF, правый задний - mRR

// Функция setSpeed задает скорость движения шасси
void setSpeed();

// Функция setAngle задает угол поворота шасси
void setAngle();

// Функция setDirection задает направление движения шасси
void setDirection();

void setDirectionAll(byte dir);

// Обеспечивает реакцию на изменения значений глобальных 
// переменных Speed, Direction и Angle
void isChanged();
