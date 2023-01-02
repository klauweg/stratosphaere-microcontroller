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

DataResult<LM75Data> LM75Sensor::getData() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(true);
  uint8_t status = Wire.requestFrom(LM75_ADDRESS, (size_t) 2, true);
  int16_t temperature = Wire.read()<<8 | Wire.read();
  return {status == 0 ? 0 : 1, LM75Data(temperature)};
}

void LM75Data::print() {
	printf("\n===[ LM75 ]===\033[K\nTemperature: %d\033[K\n\033[K", this->temperature);
}