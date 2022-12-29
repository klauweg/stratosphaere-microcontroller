#include "HIHSensor.h"
#include <iostream>
#include <Wire.h>

const uint8_t HIH_ADDRESS = 0x27;

void HIHSensor::configure() {
}

HIHData::HIHData(uint16_t humidity, uint16_t temperature) {
    this->humidity = humidity;
    this->temperature = temperature;
}

HIHData HIHSensor::getData() {
  Wire.beginTransmission(HIH_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(false);
  Wire.requestFrom(HIH_ADDRESS, (size_t) 2*2, true);
  int16_t humidity = Wire.read()<<8 | Wire.read();
  int16_t temperature = Wire.read()<<8 | Wire.read();
  return {humidity, temperature};
}