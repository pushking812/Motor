// Валидация команды
bool isValidCommand(parsedCmd* p) {
  // Проверка валидности кода команды

  Serial.print("Error: isValidCommand parsed value: ");
  Serial.println(p->value);

  if (!isValidCmdCode(p)) {
    Serial.println("Error: isValidCommand code absent");
    return false;
  }
  // Проверка валидности значения для команды

  if (!isValidValue(p)) {
    Serial.println("Error: isValidCommand wrong value");
    return false;
  }

  // Если команда прошла все проверки, то считаем её валидной
  return true;
}

bool isValidCmdCode(parsedCmd* p) {
  return (p->code == 'S' || p->code == 'D' || p->code == 'A');
}

bool isValidValue(parsedCmd* p) {
  char code = p->code;
  int value = p->value;
  
  Serial.print("Error: isValidValue parsed command: ");
  Serial.println(p->value);

  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (code == commands[i].code) {
      struct Commands command = commands[i];
      if (command.validValues != NULL && command.numValidValues > 0) {
        // проверка по массиву допустимых значений
        for (int j = 0; j < command.numValidValues; j++) {
          if (value == command.validValues[j]) return true;
        }
        return false;
      } else {
        // проверка по минимальному и максимальному значению
        if (value >= command.minValue && value <= command.maxValue) return true;
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
