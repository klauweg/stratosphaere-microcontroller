#include "LM75Sensor.h"
#include <Wire.h>

#include "debug.h"

LM75CorrectedData::LM75CorrectedData(float temperature) {
    this->temperature = temperature;
}
LM75CorrectedData::LM75CorrectedData() {
    this->temperature = 0;
}
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
  temperature = temperature >> 7;
  this->data = LM75Data(temperature);
  this->correct();
  binprint( temperature );
}

void LM75Sensor::correct() {
   ///////// warum uint!?!?!?!?
    uint16_t temperature = this->getData().getTemperature();
    this->correctedData = LM75CorrectedData{temperature/2.0f};
}

void LM75Data::print(LM75CorrectedData corrected) const {
	printf("\n===[ LM75 ]===\033[K\nRaw Temperature: %d\033[K\n\033[K\nTemperature: %f\033[K\n", this->temperature, corrected.getTemperature());
}
