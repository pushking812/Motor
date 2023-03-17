#include "Validate.h"

Commands commands = {
  { SPD, SPD_MIN, SPD_MAX, NULL, 0 },
  { DIR, DIR_STOP, DIR_BACK, validDirValues, 3 },
  { LFT, ANG_MIN, ANG_MAX, NULL, 0 },
  { RGT, ANG_MIN, ANG_MAX, NULL, 0 }
};

// Валидация команды
bool isValidCommand(parsedCmd* p) {
  // Проверка валидности кода команды

  //Serial.print("(I) isValidCommand value: ");
  //Serial.println(p->value);

  if (!isValidCmdCode(p)) {
    //Serial.println("(E) isValidCommand code not found");
    return false;
  }
  // Проверка валидности значения для команды

  if (!isValidValue(p)) {
    //Serial.println("(E) isValidCommand wrong value");
    return false;
  }

  // Если команда прошла все проверки, то считаем её валидной
  return true;
}

bool isValidCmdCode(parsedCmd* p) {
  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (commands[i].code == p->code) {
      return true;
    }
  }
  return false;
}

bool isValidValue(parsedCmd* p) {
  char code = p->code;
  int value = p->value;
  
  //Serial.print("(I) isValidValue value: ");
  //Serial.println(p->value);

  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (code == commands[i].code) {
      Command* commandPtr = &commands[i];
      if (commandPtr->validValues != NULL && commandPtr->numValidValues > 0) {
        // проверка по массиву допустимых значений
        for (int j = 0; j < commandPtr->numValidValues; j++) {
          if (value == commandPtr->validValues[j]) return true;
        }
        return false;
      } else {
        // проверка по минимальному и максимальному значению
        if (value >= commandPtr->minValue && value <= commandPtr->maxValue) return true;
        return false;
      }
    }
  }
  return false;
}

bool isDigit(char c) {
  return (c >= '0' && c <= '9');
}

bool isAlpha(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

int isNumber(const char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (!isDigit(str[i])) return 0;
  }

  return 1;
}

int isWord(const char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (!isAlpha(str[i])) return 0;
  }
  
  return 1;
}
