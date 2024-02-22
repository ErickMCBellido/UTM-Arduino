#ifndef LOAD_CELL_H
#define LOAD_CELL_H

#include "HX711.h"

HX711 scale;

uint8_t dataPin = A2;
uint8_t clockPin = A3;

void setupLoadCell(){
  scale.begin(dataPin, clockPin);
  // Initial calibration based on tests
  scale.set_scale(-674.568725);
  scale.tare(20);
}

void calibLoadCell(){
  Serial.println("Remove all weight from the loadcell and press enter\n");
  //  flush Serial input
  while (Serial.available()) Serial.read();
  while(!Serial.available()) {
    delay(100);
  }

  Serial.println("Determine zero weight offset");
  scale.tare(20);  // average 20 measurements.
  uint32_t offset = scale.get_offset();

  Serial.print("OFFSET: ");
  Serial.println(offset);
  Serial.println();


  Serial.println("Place a weight on the loadcell.");
  Serial.println("Enter the weight in (whole) grams and press enter");
  //  flush Serial input
  while (Serial.available()) Serial.read();

  uint32_t weight = 0;
  while (Serial.peek() != '\n')
  {
    if (Serial.available())
    {
      char ch = Serial.read();
      if (isdigit(ch))
      {
        weight *= 10;
        weight = weight + (ch - '0');
      }
    }
  }
  Serial.print("WEIGHT: ");
  Serial.println(weight);
  scale.calibrate_scale(weight, 20);
  float scaleV = scale.get_scale();

  Serial.print("SCALE:  ");
  Serial.println(scaleV, 8);

  Serial.print("offset: ");
  Serial.print(offset);
  Serial.print(", scale: ");
  Serial.print(scaleV, 8);
  Serial.println("\n");
}

void setLCCalib(long offset, float scaleV){
  scale.set_offset(offset);
  scale.set_scale(scaleV);
}

void getLCCalib(){
  long offset = scale.get_offset();
  float scaleV = scale.get_scale();
  Serial.print(offset);
  Serial.print(",");
  Serial.println(scaleV,8);
}

void readLoadCell(){
  loadCReading = scale.get_units(1);
}

#endif //LOAD_CELL_H