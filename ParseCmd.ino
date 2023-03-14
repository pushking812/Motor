// Функция getCmd получает и обрабатывает команды последовательного порта,
// задает значения глобальных переменных Speed, Direction и Angle
int setCmd() {
  // Считываем команду с последовательного порта
  char* cmd = readStringUntil('\n', CMD_TIMEOUT);
  if (cmd == NULL) {
    Serial.println("Error: setCmd Command read error");
    return -1;
  }

  //Serial.print("Info: setCmd cmd: "); Serial.println(*cmd);

  parsedCmd* p = parseCmd(cmd);
  if (p == nullptr) {
    Serial.println("Error: setCmd Command parsing error");
    return -1;
  }

  //Serial.print("Info: setCmd code, value: "); Serial.print(p->code);
  //Serial.print(", "); Serial.println(p->value);

  // Проверяем валидность команды
  if (!isValidCommand(p)) {
    Serial.println("Error: setCmd Invalid command");
    delete p; // освобождаем выделенную память
    return -1;
  }

  // Записываем значение команды в соответствующую переменную
  // Speed, Direction или Angle
  runCmd(p);

  delete p;

  return 0;
}

// получаем из последовательного порта строку ограниченную указанным символом
char* readStringUntil(char terminator, unsigned int timeout) {
  static char buffer[MAXCMDLEN]; // объявляем буфер статическим, чтобы он не уничтожался после выхода из функции
  byte i = 0;
  Serial.setTimeout(timeout); // устанавливаем таймаут
  while (Serial.available()) {
    if (i < MAXCMDLEN - 1) {
      char c = Serial.read();
      if (c == terminator) {
        buffer[i] = '\0'; // ставим нулевой символ в конец строки
        //Serial.print("Info: readStringUntil buffer: ");
        //Serial.println(buffer);
        return buffer; // возвращаем указатель на буфер, который всегда имеет значение
      } else {
        buffer[i] = c;
        i++;
      }
    }
  }
  // если не был получен терминирующий символ, то возвращаем NULL
  return NULL;
}

parsedCmd* parseCmd(const char* cmd) {
  //Serial.print("Info: parseCmd cmd: "); Serial.println(*cmd);

  // Проверка на пустую строку
  if (strlen(cmd) == 0) {
    Serial.println("Error: parseCmd command absent");
    return nullptr;
  }

  // Проверка на наличие символа ','
  char* colonPtr = strchr(cmd, ',');
  if (colonPtr == NULL) {
    Serial.println("Error: parseCmd comma absent");
    return nullptr;
  }

  char code = parseCmdCode(cmd);
  if (code == -1) {
    Serial.println("Error: parseCmd wrong code format");
    return nullptr;
  }

  int value = parseCmdValue(cmd, IS_NUMBER | IS_WORD);
  if (value == -1) {
    Serial.println("Error: parseCmd wrong value format");
    return nullptr;
  }

  //Serial.print("Info: parseCmd code, value: ");
  //Serial.print(code);Serial.print(", ");Serial.println(value);

  parsedCmd* p = new parsedCmd{code, value};

  return p;
}



// Функция для парсинга кода команды из строки
char parseCmdCode(const char* cmd) {
  if (cmd[1] != ',') return -1;
  return cmd[0];
}

// Функция для парсинга значения команды из строки
// для проверки, что значение либо число, либо слово:
// parseCmdValue(cmd, IS_NUMBER | IS_WORD)
int parseCmdValue(const char* cmd, byte flags) {
  //Serial.print("Info: parseCmdValue cmd, flags: "); Serial.print(cmd);
  //Serial.print(", "); Serial.println(flags);
  char* value = strchr(cmd, ',');

  if (value == NULL) {
    Serial.println("Error: parseCmdValue absent value");
    return -1;
  }
  value++; // переход к символу после запятой

  //Serial.print("Info: parseCmdValue value: ");
  //Serial.println(value);

  if (*value == '\0') {
    Serial.println("Error: parseCmdValue zero value");
    return -1;
  }

  // Проверка значения на то, что оно положительное число
  if (!isNumber(value)) {
    Serial.println("Error: parseCmdValue value is not number");
    return -1;
  }

  bool isValueCorrect = false;

  if ((bitRead(flags, IS_NUMBER) & isNumber(value)) == 1) isValueCorrect = true;
  if ((bitRead(flags, IS_WORD) & isWord(value)) == 1) isValueCorrect = true;

  if (!isValueCorrect) {
    /*Serial.print("Error: parseCmdValue incorrect value: ");
      if (flags & IS_NUMBER) {
      Serial.println("not number ");
      }
      if (flags & IS_WORD) {
      Serial.println("not word ");
      } */
    return -1;
  }

  //Serial.print("Info: Result of validation: true");
  return atoi(value);
}

// Записывает значение команды в соответствующую переменную
// Speed, Direction или Angle
void runCmd(parsedCmd* p) {
  switch (p->code) {
    case SPD:
      prevSpeed = Speed;
      Speed = p->value;
      break;
    case DIR:
      prevDirection = Direction;
      Direction = p->value;
      break;
    case LFT:
      prevAngle = Angle;
      Angle = -p->value;
      break;
    case RGT:
      prevAngle = Angle;
      Angle = p->value;
      break;
    default:
      break;
  }
}
