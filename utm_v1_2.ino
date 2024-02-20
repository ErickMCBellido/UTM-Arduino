#include "header_lib.h"
#include "motor_op.h"
#include "load_cell.h"
#include "serial_com.h"
#include "operations.h"

void setup() {
  setupMotor();
  setupLoadCell();
  setupSerial();
}

void loop() {
  Operation op;
  while(true){
    op = readCommand();
    //if(op != 0) Serial.println(op);
    opExecute(op);
  }
}