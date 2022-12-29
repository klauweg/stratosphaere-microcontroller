#include "LM75Sensor.h"
#include <iostream>
#include <Wire.h>

const uint8_t LM75_ADDRESS = 0x4f;

LM75Data::LM75Data(uint16_t temperature) {
    this->temperature = temperature;
}

void LM75Sensor::configure() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission(false);
}

LM75Data LM75Sensor::getData() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(false);
  Wire.requestFrom(LM75_ADDRESS, (size_t) 2, true);
  int16_t temperature = Wire.read()<<8 | Wire.read();
  return {temperature};
}