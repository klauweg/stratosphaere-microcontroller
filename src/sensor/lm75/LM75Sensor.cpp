#include "LM75Sensor.h"
#include <Wire.h>

LM75Data::LM75Data(int16_t temperature) {
    this->temperature = temperature;
}
LM75Data::LM75Data() {
    this->temperature = 0;
}

void LM75Sensor::configure() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void LM75Sensor::measure() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(true);
  this->status = Wire.requestFrom(LM75_ADDRESS, (size_t) 2, true);
  int16_t temperature = Wire.read()<<8 | Wire.read();
  this->data = LM75Data(temperature);
}

void LM75Data::print() {
	printf("\n===[ LM75 ]===\033[K\nTemperature: %d\033[K\n\033[K", this->temperature);
}