#pragma GCC push_options
#pragma GCC optimize ("Os")

#include <core.h> // Required by cpu
#include <cpu.h>

#pragma GCC pop_options

const int motor[] = {3, 9, 10, 11}; // Motor PWM Difital Pins

const int speed[] = {A0, A1, A2, A3}; // Motor speed sensor on Analog Pins

int speed_curr[] = {LOW, LOW, LOW, LOW};

int speed_last[] = {LOW, LOW, LOW, LOW};

const int start_duty[] = {50, 50, 50, 50}; // start value of PWM

const int delta_duty[] = {10, 20, 30, 40}; // delta of changing PWM

int duty[] = {start_duty[0], start_duty[1], start_duty[2], start_duty[3]}; // current PWM

bool forward[] = {true, true, true, true}; // increase (true) or reducing (false)

unsigned long start[4];
unsigned long stop[4];

void setup () {
  Serial.begin(9600);

  while (!Serial);

  //pinMode(motor[0], OUTPUT); // Not required for PWM
  pinMode(speed[0], INPUT_PULLUP);
  pinMode(speed[1], INPUT_PULLUP);
  pinMode(speed[2], INPUT_PULLUP);
  pinMode(speed[3], INPUT_PULLUP);

  unsigned long ms = millis();
  start[0] = ms;
  start[1] = ms;
  start[2] = ms;
  start[3] = ms;
}

int numImp[] = {0, 0, 0, 0};

void loop() {
  int err =
    getWheelSpeed(0) ||
    getWheelSpeed(1) ||
    getWheelSpeed(2) ||
    getWheelSpeed(3); // Ерунда с подсчетом импульсов, нужно считать кол-во импульсов в ед. времени
  //int err=getWheelSpeed(3);
  while (err);

  //int maxSpeed = max(numImp[0], max(numImp[1],max(numImp[2],numImp[3])));
  //int minSpeed = min(numImp[0], min(numImp[1],min(numImp[2],numImp[3])));

  int mx = getWheelWithMaxSpeed(numImp);
  //Serial.print("Max revolving: "); Serial.println(mx);

  //if(numImp[0]==maxSpeed) {
  //   decreaseSpeed(0);
  //}

  //delay(250);
}

int getWheelWithMaxSpeed(int n[]) {
  byte maxIndex = 0;
  int maxValue = n[maxIndex];

  for (byte i = 1; i < 4; i++) {
    if (n[i] > maxValue) {
      maxValue = n[i];
      maxIndex = i;
    }
  }
  return maxIndex;
}

void setSpeed(int m) {
  analogWrite(motor[m], duty[m]);
}

int getWheelSpeed(int m) {
  speed_curr[m] = digitalRead(speed[m]);

  if (speed_curr[m] == HIGH && speed_last[m] == LOW) {
    //Serial.print(speed_curr[m]); Serial.print(":");
    //Serial.print(speed_last[m]); Serial.print(":");
    //Serial.println(++numImp[m]);
    numImp[m]++;
  }

  if ((millis() - start[m]) > 1000) {
    Serial.print(m); Serial.print(": not revolving ");
    Serial.println(millis() - start[m]);
    return 1;
  }

  if (numImp[m] >= 10) {
    Serial.print(m); Serial.print(":");
    Serial.println(millis() - start[m]);
    numImp[m] = 0;
    start[m] = millis();
  }

  speed_last[m] = speed_curr[m];

  return 0;
  //memcpy(&speed_last, &speed_curr, sizeof(speed_curr[0])*4);
}


/*
  setSpeed(0);
  setSpeed(1);
  setSpeed(2);
  setSpeed(3);
*/

/*
  debug_motor(0);
  debug_motor(1);
  debug_motor(2);
  debug_motor(3);
  Serial.println();
*/

//getSpeed();

void getSpeed() {
  unsigned long start = millis();
  unsigned long stop = 0;

  Serial.print("start: "); Serial.println(start);

  bool print = true;
  while ((millis() - start) <= 1000) {
    if (((millis() - start) % 100) == 0) {
      if (print) Serial.print(".");
      print = false;
    } else {
      print = true;
    }
  }

  stop = millis();

  Serial.println();
  Serial.print("stop: "); Serial.println(stop);
  Serial.print("time,ms: "); Serial.println(stop - start);
  Serial.println();

  // int sensorVal = digitalRead(2);
  // prot
}

void debug_motor(int m) {
  Serial.print(m); Serial.print(":"); Serial.print(forward[m]);
  Serial.print(" "); Serial.print(duty[m]); Serial.print("; ");
}



void setSpeed(int m, bool test) {
  if (forward[m] == true && duty[m] <= 256) {
    if ((256 - duty[m]) >= delta_duty[m]) duty[m] += delta_duty[m];
    else {
      duty[m] = 256;
      forward[m] = false;
    }
  }
  if (forward[m] == false && duty[m] >= 0) {
    if ((duty[m] - delta_duty[m]) >= 0) duty[m] -= delta_duty[m];
    else {
      duty[m] = 0;
      forward[m] = true;
    }
  }

  analogWrite(motor[m], duty[m]);
}
