#ifndef MOTOR_OP_H
#define MOTOR_OP_H

// The pin 9 is used for the pulse signal, corresponding to timer 1 output compare

#define dirP 8  // pin 8 for direction
#define enP 10  // pin 10 for enable

void setupMotor(){
  pinMode(dirP, OUTPUT);
  digitalWrite(dirP, LOW);
  pinMode(enP, OUTPUT);
  digitalWrite(enP, LOW);
  pinMode(9, OUTPUT); //OC1A
  //digitalWrite(9, HIGH);
  enTimerOC();
}

void setEnState(bool state){
  enState = state;
  digitalWrite(enP, state);
}

void reverseRotation(bool state){
  invRot = state;
}

void setDirection(bool dir){
  //rotDir = dir;
  if(invRot) dir = !dir;
  digitalWrite(dirP, dir);
}

void startRotation(MovDir dir,int cmpV){
  setDirection(dir);
  startTimer(cmpV); //10-20 max velocity on complete steps
}

void stopRotation(){
  stopTimer();
}

#endif //MOTOR_OP_H