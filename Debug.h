#pragma once

#include "Arduino.h"

// сообщения отладки
extern const byte DEBUG; // `0` не выводить отладочные сообщения
                         // `1` выводить все отладочные сообщения
                         // `2` выводить только сообщения об ошибках
                    
extern const char* DBG_PRE_ERR; // префикс ошибок
extern const char* DBG_PRE_INF; // префикс информационных сообщений
extern const char* DBG_MSG_INVVAL;
extern const char* DBG_MSG_VAL;
extern const char* DBG_MSG_CR;
extern const char* DBG_MSG_CP;
extern const char* DBG_MSG_CV;
extern const char* DBG_MSG_CC;
extern const char* DBG_MSG_CVL;
extern const char* DBG_MSG_CA;
extern const char* DBG_MSG_VA;
extern const char* DBG_MSG_VZ;
extern const char* DBG_MSG_VNN;
extern const char* DBG_MSG_CMA;

void DebugMsg(char *pre, char *func, char *msg, char *var, char *val, bool last);

void DebugMsg(char *pre, char *func, char *msg, char *var, int val, bool last);

void DebugMsg(char *pre, char *func, char *msg, char *var, float val, bool last);