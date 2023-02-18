

const int MOTOR = 3;  //Motor on Digital Pin 9
const int SPEED = 2;  //Motor on Digital Pin 9

const int I = 50;
const int DELTA_I = 15;

int i = I;
bool forward = true;

void setup()
{
  Serial.begin(9600);

  pinMode(MOTOR, OUTPUT);
  pinMode(SPEED, INPUT_PULLUP);
}

// 1 считываем и возвращаем скорость колеса, в обороты/сек
// 2 скорость выводим в терминал
// 3 крутим колесо с переменной скоростю
void loop() {
  setSpeed();
  Serial.print(forward);
  Serial.print(" ");
  Serial.println(i);
  
  getSpeed();

  delay(1000);
}

void setSpeed() {
  if (forward == true && i <= 256) {
    if ((256 - i) >= DELTA_I) i += DELTA_I;
    else {
      i = 256;
      forward = false;
    }
  }
  if (forward == false && i >= 0) {
    if ((i-DELTA_I) >= 0) i -= DELTA_I;
    else {
      i= 0;
      forward = true;
    }
  }
  
  analogWrite(MOTOR, i);
}

int getSpeed() {
  unsigned long start = millis();
  unsigned long stop=0;

  Serial.print("start: ");
  while ((start - millis()) <= 1000);
  stop = millis();
  
  Serial.print("stop: ");
  Serial.print("time,ms: ");
  Serial.println(start-stop);
 
  // int sensorVal = digitalRead(2);
  // notepad++
}
