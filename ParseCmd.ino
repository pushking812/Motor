

// Функция getCmd получает и обрабатывает команды последовательного порта,
// задает значения глобальных переменных Speed, Direction и Angle
int setCmd() {
  const char* DBG_FUNC="setCmd";

  // Считываем команду с последовательного порта
  char* cmd = readStringUntil('\n', CMD_TIMEOUT);
  if (cmd == NULL) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_CC, "cmd", *cmd, true);
    return -1;
  }

  //Serial.print("(I) setCmd cmd: "); Serial.println(*cmd);

  parsedCmd* p = parseCmd(cmd);
  if (p == nullptr) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_CP, "cmd", *cmd, true);
    return -1;
  }

  //Serial.print("(I) setCmd code, value: "); Serial.print(p->code);
  //Serial.print(", "); Serial.println(p->value);

  // Проверяем валидность команды
  if (!isValidCommand(p)) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_CV, "cmd", *cmd, false);
    DebugMsg(DBG_PRE_ERR, "", "", "p->code", p->code, false);
    DebugMsg(DBG_PRE_ERR, "", "", "p->value", p->value, true);

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
  const char* DBG_FUNC="readStringUntil";

  static char buffer[MAXCMDLEN]; // объявляем буфер статическим, чтобы он не уничтожался после выхода из функции
  byte i = 0;
  Serial.setTimeout(timeout); // устанавливаем таймаут
  while (Serial.available()) {
    if (i < MAXCMDLEN - 1) {
      char c = Serial.read();
      if (c == terminator) {
        buffer[i] = '\0'; // ставим нулевой символ в конец строки
        //Serial.print("(I) readStringUntil buffer: ");
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
  const char* DBG_FUNC="parseCmd";
  //Serial.print("(I) parseCmd cmd: "); Serial.println(*cmd);

  // Проверка на пустую строку
  if (strlen(cmd) == 0) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_CA, "cmd", *cmd, true);    
    return nullptr;
  }

  // Проверка на наличие символа ','
  char* colonPtr = strchr(cmd, ',');
  if (colonPtr == NULL) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_CMA, "cmd", *cmd, true);    
    return nullptr;
  }

  char code = parseCmdCode(cmd);
  if (code == -1) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_CC, "cmd", *cmd, true);      
    return nullptr;
  }

  int value = parseCmdValue(cmd, IS_NUMBER | IS_WORD);
  if (value == -1) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_CV, "cmd", *cmd, true);  
    return nullptr;
  }

  //Serial.print("(I) parseCmd code, value: ");
  //Serial.print(code);Serial.print(", ");Serial.println(value);

  parsedCmd* p = new parsedCmd{code, value};

  return p;
}

// Функция для парсинга кода команды из строки
char parseCmdCode(const char* cmd) {
  const char* DBG_FUNC="parseCmdCode";

  if (cmd[1] != ',') return -1;
  return cmd[0];
}

// Функция для парсинга значения команды из строки
// для проверки, что значение либо число, либо слово:
// parseCmdValue(cmd, IS_NUMBER | IS_WORD)
int parseCmdValue(const char* cmd, byte flags) {
  const char* DBG_FUNC="parseCmdValue";
  //Serial.print("(I) parseCmdValue cmd, flags: "); Serial.print(cmd);
  //Serial.print(", "); Serial.println(flags);
  char* value = strchr(cmd, ',');

  if (value == NULL) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_VA, "cmd", *cmd, true);  
    return -1;
  }
  value++; // переход к символу после запятой

  //Serial.print("(I) parseCmdValue value: ");
  //Serial.println(value);

  if (*value == '\0') {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_VZ, "cmd", *cmd, true);  
    return -1;
  }

  // Проверка значения на то, что оно положительное число
  if (!isNumber(value)) {
    DebugMsg(DBG_PRE_ERR, DBG_FUNC, DBG_MSG_VNN, "cmd", *cmd, true); 
    return -1;
  }

  bool isValueCorrect = false;

  if ((bitRead(flags, IS_NUMBER) & isNumber(value)) == 1) isValueCorrect = true;
  if ((bitRead(flags, IS_WORD) & isWord(value)) == 1) isValueCorrect = true;

  if (!isValueCorrect) {
    /*Serial.print("(E) parseCmdValue incorrect value: ");
      if (flags & IS_NUMBER) {
      Serial.println("not number ");
      }
      if (flags & IS_WORD) {
      Serial.println("not word ");
      } */
    return -1;
  }

  //Serial.print("(I) Result of validation: true");
  return atoi(value);
}

// Записывает значение команды в соответствующую переменную
// Speed, Direction или Angle
void runCmd(parsedCmd* p) {
  const char* DBG_FUNC = "runCmd";

  if (p->code == SPD) {
    prevSpeed = Speed;
    Speed = p->value;
  }
  if (p->code == DIR) {
    prevDirection = Direction;
    Direction = p->value;
  }
  if (p->code == LFT) {
    prevAngle = Angle;
    Angle = -p->value;
  }
  if (p->code == RGT) {
    prevAngle = Angle;
    Angle = p->value;
  }
}
