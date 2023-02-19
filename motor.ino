#pragma GCC push_options
#pragma GCC optimize ("Os")

#include <core.h> // Required by cpu
#include <cpu.h>

#pragma GCC pop_options

const int motor[] = {3, 9, 10, 11}; // Motor PWM Difital Pins 

const int speed[] = {A0, A1, A2, A3}; // Motor speed sensor on Analog Pins 

const int start_duty[] = {50, 50, 50, 50}; // start value of PWM 

const int delta_duty[] = {10, 20, 30, 40}; // delta of changing PWM 

int duty[] = {start_duty[0], start_duty[1], start_duty[2], start_duty[3]}; // current PWM

bool forward[] = {true, true, true, true}; // increase (true) or reducing (false) 

void setup () {
  Serial.begin(9600);

  //pinMode(motor[0], OUTPUT); // Not required for PWM
  pinMode(speed[0], INPUT_PULLUP);
  pinMode(speed[1], INPUT_PULLUP);
  pinMode(speed[2], INPUT_PULLUP);
  pinMode(speed[3], INPUT_PULLUP);
}

void loop() {
  setSpeed(0);
  setSpeed(1);
  setSpeed(2);
  setSpeed(3);
  debug_motor(0);
  debug_motor(1);
  debug_motor(2);
  debug_motor(3);
  Serial.println();
  
  //getSpeed();

  delay(250);
}

void debug_motor(int m) {
   Serial.print(m); Serial.print(":"); Serial.print(forward[m]); 
   Serial.print(" "); Serial.print(duty[m]); Serial.print("; ");
}

void setSpeed(int m) {
  if (forward[m] == true && duty[m] <= 256) {
    if ((256 - duty[m]) >= delta_duty[m]) duty[m] += delta_duty[m];
    else {
      duty[m] = 256;
      forward[m] = false;
    }
  }
  if (forward[m] == false && duty[m] >= 0) {
    if ((duty[m]-delta_duty[m]) >= 0) duty[m] -= delta_duty[m];
    else {
      duty[m]= 0;
      forward[m] = true;
    }
  }
  
  analogWrite(motor[m], duty[m]);
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
  // prot
}
