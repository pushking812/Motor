// максимальное количество символов в коде команды кода
const byte MAXCODLEN=1;

// максимальная длина команды
const byte MAXCMDLEN=16;

// Функция для парсинга кода команды из строки
char parseCmdCode(const char* cmd) {
  switch (cmd[0]) {
    case 'S':
    case 'D':
    case 'A':
      return cmd[0];
    default:
      Serial.print("Error: Unknown command code ");
      Serial.println(cmd[0]);
      return -1;
  }
}

int isDigit(char c) {
  return (c >= '0' && c <= '9' ? 1 : 0);
}

int isAlpha(char c) {
  return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? 1 : 0);
}

bool isNumber(const char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (!isdigit(str[i])) return false;
  }
  return true;
}

bool isWord(const char* str) {
  for (int i = 0; str[i] != '\0'; i++) {
    if (!isAlpha(str[i])) return false;
  }
  return true;
}

const byte IS_NUMBER = 0x01;
const byte IS_WORD = 0x02;

// Функция для парсинга значения команды из строки
// для проверки, что значение либо число, либо слово:
// parseCmdValue(cmd, IS_NUMBER | IS_WORD)
int parseCmdValue(const char* cmd, byte flags) {
  Serial.print("Info: parseCmdValue() input: "); Serial.println(cmd);

  char* comma = strchr(cmd, ',');
  if (comma == NULL) {
    Serial.println("Error: Invalid command format");
    return -1;
  }
  
  char* value = comma + 1;
  
  Serial.println("Info: parseCmdValue() parsed value: "); Serial.println(value);
  
  if (*value == '\0') {
  Serial.println("Error: Command value is absent");
  return -1;
  }
  
  bool isValueCorrect = false;
  
  if (flags & IS_NUMBER && isNumber(cmd)) isValueCorrect = true;
  if (flags & IS_WORD && isWord(cmd)) isValueCorrect = true;

  if(!isValueCorrect) {
  Serial.print("Error: Command value is incorrect: ");
  if(flags & IS_NUMBER) {
    Serial.println("not number ");
  }
  if(flags & IS_WORD) {
    Serial.println("not word ");
  }
  return -1;
  }

  return atoi(cmd);
}

// Функция для проверки валидности значения команды
bool isValidValue(Commands& cmd, int val) {
  if (cmd.validValues != NULL && cmd.numValidValues > 0) {
    // проверка по массиву допустимых значений
    for (int j = 0; j < cmd.numValidValues; j++) {
      if (val == cmd.validValues[j]) return true;
    }
  } else {
    // проверка по минимальному и максимальному значению
    if (val >= cmd.minValue && val <= cmd.maxValue) return true;
  }
  return false;
}

// получаем из последовательного порта строку ограниченную указанным символом
char* readStringUntil(char terminator, unsigned int timeout) {
  static char buffer[MAXCMDLEN];
  byte i = 0;
  Serial.setTimeout(timeout); // устанавливаем таймаут
  while (i < MAXCMDLEN-1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == terminator) {
        buffer[i] = '\0';
        return buffer;
      } else {
        buffer[i] = c;
        i++;
      }
    }
  }
  return NULL;
}

// получаем из последовательного порта строку ограниченную указанным символом
char* readStringUntil(char terminator, unsigned int timeout) {
  static char buffer[MAXCMDLEN];
  byte i = 0;
  Serial.setTimeout(timeout); // устанавливаем таймаут
  while (i < MAXCMDLEN-1) {
    if (Serial.available()) {
      char c = Serial.read();
      if (c == terminator) {
        buffer[i] = '\0';
      } else {
        buffer[i] = c;
        i++;
      }
    }
  }
  return buffer; // изменено на возврат указателя на буфер, который всегда имеет значение
}

// const unsigned int CMD_TIMEOUT=1000;
// Функция parseCmd получает и обрабатывает команды последовательного порта,
// задает значения глобальных переменных Speed, Direction и Angle
// const unsigned int CMD_TIMEOUT=1000;
// Функция parseCmd получает и обрабатывает команды последовательного порта,
// задает значения глобальных переменных Speed, Direction и Angle
int parseCmd() {
  // считываем команду с последовательного порта
  char* cmd = readStringUntil('\n', CMD_TIMEOUT); 

  if (!isValidCommand(cmd)) {
    Serial.println("Error: Invalid command");
    return -1;
  }

  char code = parseCmdCode(cmd); // получаем код команды
  int value = parseCmdValue(cmd, IS_NUMBER | IS_WORD); // получаем значение команды

  // искать команду в массиве и вызвать её обработчик
  for (int i = 0; i < NUM_COMMANDS; i++) {
    if (code == commands[i].code) {
      if (isValidValue(commands[i], value)) {
        isChanged(code);
        return 0;
      } else {
        Serial.println("Error: Invalid value");
        return -1;
      }
    }
  }

  Serial.println("Error: Unknown command");
  return -1;
}

bool isValidCommand(const char* cmd) {
  // Проверка на пустую строку
  if (strlen(cmd) == 0) {
    return false;
  }

  // Проверка на наличие символа ','
  char* colonPtr = strchr(cmd, ',');
  if (colonPtr == NULL) {
    return false;
  }

  // Проверка на корректность кода команды
  char code = *(colonPtr - 1);
  if (parseCmdCode(&code) == -1) {
    return false;
  }

  // Проверка на корректность значения команды
  int value = parseCmdValue(cmd, IS_NUMBER | IS_WORD);
  if (value == -1) {
    return false;
  }

  // Проверка на наличие завершающего символа '\n'
  if (*(cmd + strlen(cmd) - 1) != '\n') {
    return false;
  }

  // Если все проверки пройдены успешно, то команда валидна
  return true;
}
