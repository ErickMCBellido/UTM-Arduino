//#include "HardwareSerial.h"
//#include "Arduino.h"
#ifndef OPERATIONS_H
#define OPERATIONS_H

void opExecute(Operation op);

void eStop(){
  noInterrupts();
  stopRotation();
  setEnState(false);
  interrupts();
}

void stopMotor(){
  stopRotation();
}

void enableMotor(){
  setEnState(true);
}

void disableMotor(){
  setEnState(false);
}

void clearConuter(){
  stepCount = 0;
}

void reverseMotor(){
  reverseRotation(true);
}

void noReverseMotor(){
  reverseRotation(false);
}

void calibrateLoadCell(){
  calibLoadCell();
}

void fastMov(MovDir dir){
  Operation op;
  startRotation(dir,20);
  while((op = stopC()) == NO_OP){
  }
  opExecute(op);
}

void stepFastMov(MovDir dir, int numSteps){
  Operation op;
  //if(dir == DOWN) numSteps = -numSteps;
  //targetStp = stepCount + numSteps;
  targetStp = stepCount + numSteps;
  stpTrgActive = true;
  startRotation(dir,20);
  while((op = stopC()) == NO_OP){
    if(!stpTrgActive) break;
  }
  if(stpTrgActive) opExecute(op);
}

void controlledSpeedMov(MovDir dir, int cmpV){
  Operation op;
  startRotation(dir,cmpV);
  while((op = stopC()) == NO_OP) {}
  opExecute(op);
}

void stepSpeedMov(MovDir dir, int numSteps, int cmpV){
  Operation op;
  //if(dir == DOWN) numSteps = -numSteps;
  //targetStp = 0 + numSteps;
  targetStp = stepCount + numSteps;
  stpTrgActive = true;
  startRotation(dir,cmpV);
  while((op = stopC()) == NO_OP){
    if(!stpTrgActive) break;
  }
  if(stpTrgActive) opExecute(op);
}

void getStepCount(){
  Serial.println(stepCount);
}

void getLoadCellReading(){
  readLoadCell();
  Serial.println(loadCReading);
}

void setThreshold(float thr){
  threshold = thr;
}

void getThreshold(){
  Serial.println(threshold);
}

void setCriteria(int val){
  stopCriteria = (val)? true:false;
}

void setCalibration(long offset_v, float scale_v){
  setLCCalib(offset_v,scale_v);
}

void getCalibration(){
  getLCCalib();
}

void getStatus(){
  //Serial.print(rotDir);
  //Serial.print(",");
  Serial.print(invRot);
  Serial.print(",");
  Serial.print(enState);
  Serial.print(",");
  Serial.print(threshold);
  Serial.print(",");
  Serial.print(stopCriteria);
  //Serial.print(",");
  //Serial.println(tensileLoadSign);
}

void setDeadZone(float deadZ){
  deadZone = deadZ;
}

void getDeadZone(){
  Serial.println(deadZone);
}

////////////////////////////////////////////////////////////////////////////////////////////

void tensileTest(int cmpV){
  float ref;

  readLoadCell();
  float lastReading = loadCReading;
  float initReading = loadCReading;
  long int tmpStp = 0;
  Operation state = NO_OP;
  stepCount = 0;

  Serial.print(stepCount);
  Serial.print(",");
  Serial.println(loadCReading,4);

  startRotation(UP,cmpV);

  while( !(state=stopC()) ){
    if(!stepLecture){
      tmpStp = stepCount;
      readLoadCell();

      /*
      if(tensileLoadSign){
        ref = (stopCriteria)? (lastReading * threshold):(lastReading * (1 - threshold));
        if(loadCReading < ref) break;
      }
      else{
        ref = (stopCriteria)? (lastReading * threshold):(lastReading * (1 - threshold));
        if (loadCReading > ref) break;
      }

      // this condition only works for tensileLoadSign == true
      if(lastReading > (initReading + deadZone)){
        if(loadCReading < (initReading + deadZone) && loadCReading > (initReading - deadZone)) break;
      }
      */

      ref = (stopCriteria)? (lastReading * threshold):(lastReading * (1 - threshold));
      if(loadCReading < ref) break;

      if(lastReading > (initReading + deadZone)){
        if(loadCReading < (initReading + deadZone) && loadCReading > (initReading - deadZone)) break;
      }

      Serial.print("<");
      Serial.print(tmpStp);
      Serial.print(",");
      Serial.print(loadCReading,4);
      Serial.print(">\n");
      stepLecture = true;

      lastReading = loadCReading;
    }
  }
  if(state == NO_OP){
    Serial.print("<TEST_ENDED>\n");
    opExecute(STOP);
  }
  else{
    opExecute(state);
    //Serial.print("<TEST_STOPED>\n");
  }
  
}

////////////////////////////////////////////////////////////////////////////////////////////

void opExecute(Operation op){
  if(op == E_STOP){
    eStop();
    return;
  }
  else if(op == STOP){
    stopMotor();
    return;
  }
  else if(op == EN_MOTOR){
    enableMotor();
    return;
  }
  else if(op == DIS_MOTOR){
    disableMotor();
    return;
  }
  else if(op == CLR_SC){
    clearConuter();
    return;
  }
  else if(op == INV_ROT){
    reverseMotor();
    return;
  }
  else if(op == NO_INV_ROT){
    noReverseMotor();
    return;
  }
  else if(op == CALIBRATE){
    calibrateLoadCell();
    return;
  }
  else if(op == F_UP){
    fastMov(UP);
    return;
  }
  else if(op == F_DOWN){
    fastMov(DOWN);
    return;
  }
  else if(op == S_UP){
    stepFastMov(UP, intTokens[0]);
    return;
  }
  else if(op == S_DOWN){
    stepFastMov(DOWN, intTokens[0]);
    return;
  }
  else if(op == C_UP){
    controlledSpeedMov(UP,intTokens[0]);
    return;
  }
  else if(op == C_DOWN){
    controlledSpeedMov(DOWN,intTokens[0]);
    return;
  }
  else if(op == SC_UP){
    stepSpeedMov(UP,intTokens[0], intTokens[1]);
    return;
  }
  else if(op == SC_DOWN){
    stepSpeedMov(DOWN,intTokens[0],intTokens[1]);
    return;
  }
  else if(op == T_TEST){
    tensileTest(intTokens[0]);
    return;
  }
  else if(op == G_SC){
    getStepCount();
    return;
  }
  else if(op == G_LCR){
    getLoadCellReading();
    return;
  }
  else if(op == S_THR){
    setThreshold(floatTokens[0]);
    return;
  }
  else if(op == G_THR){
    getThreshold();
    return;
  }
  else if(op == S_CRITERIA){
    setCriteria(intTokens[0]);
    return;
  }
  else if(op == S_CALIBRATION){
    setCalibration(readedOffset,readedScale);
    return;
  }
  else if(op == G_CALIBRATION){
    getCalibration();
    return;
  }
  else if(op == G_STATUS){
    getStatus();
    return;
  }
  else if(op == S_DZ){
    setDeadZone(floatTokens[0]);
    return;
  }
  else if(op == G_DZ){
    getDeadZone();
    return;
  }
}

#endif //OPERATIONS_H
