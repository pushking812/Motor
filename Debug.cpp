#include "Arduino.h"
#include "Debug.h"

const byte DEBUG=1;

const char* DBG_PRE_ERR="(E)"; // префикс ошибок
const char* DBG_PRE_INF="(I)"; // префикс информационных сообщений
const char* DBG_MSG_INVVAL="invalid value";
const char* DBG_MSG_VAL="current value";
const char* DBG_MSG_CR="command hasn't read";
const char* DBG_MSG_CP="command hasn't parsed";
const char* DBG_MSG_CV="command hasn't validated";
const char* DBG_MSG_CC="wrong command code";
const char* DBG_MSG_CVL="wrong command value";
const char* DBG_MSG_CA="command is absent";
const char* DBG_MSG_VA="command value is absent";
const char* DBG_MSG_VZ="zero command value";
const char* DBG_MSG_VNN="command value isn't number";
const char* DBG_MSG_CMA="comma in command is absent";

void DebugMsg(char *pre, char *func, char *msg, char *var, char *val, bool last) {
  if (DEBUG == 0 || (DEBUG == 2 && pre[1] != DBG_PRE_ERR[1])) {
    return;
  }
    
  if (func[0] != '\0') {
    Serial.print(pre);  Serial.print(" ");
  }
   
  Serial.print(func); Serial.print(" ");
  Serial.print(msg);  Serial.print(": ");    
  Serial.print(var);  Serial.print("="); Serial.print(val);

  if (last) Serial.println();
}

void DebugMsg(char *pre, char *func, char *msg, char *var, int val, bool last) {
  if (DEBUG == 0 || (DEBUG == 2 && pre[1] != DBG_PRE_ERR[1])) return;

  if (func[0] != '\0') {
    Serial.print(pre);  Serial.print(" ");
  }
   
  Serial.print(func); Serial.print(" ");
  Serial.print(msg);  Serial.print(": ");    
  Serial.print(var);  Serial.print("="); Serial.print(val);

  if (last) Serial.println();
}

void DebugMsg(char *pre, char *func, char *msg, char *var, float val, bool last) {
  if (DEBUG == 0 || (DEBUG == 2 && pre[1] != DBG_PRE_ERR[1])) return;

  if (func[0] != '\0') {
    Serial.print(pre);  Serial.print(" ");
  }
   
  Serial.print(func); Serial.print(" ");
  Serial.print(msg);  Serial.print(": ");    
  Serial.print(var);  Serial.print("="); Serial.print(val);

  if (last) Serial.println();
}