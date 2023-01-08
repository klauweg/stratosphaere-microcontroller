#include "HIHSensor.h"
#include <Wire.h>

void HIHSensor::configure() {
}

HIHData::HIHData(int16_t humidity, int16_t temperature) {
    this->humidity = humidity;
    this->temperature = temperature;
}
HIHData::HIHData() {
    this->humidity = 0;
    this->temperature = 0;
}

void HIHSensor::measure() {
  Wire.beginTransmission(HIH_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(true);
  this->status = Wire.requestFrom(HIH_ADDRESS, (size_t) 2*2, true);
  int16_t humidity = Wire.read()<<8 | Wire.read();
  int16_t temperature = Wire.read()<<8 | Wire.read();
  this->data = HIHData(humidity, temperature);
}

void HIHData::print() {
	printf("\n===[ HIH ]===\033[K\nTemperature: %d\033[K\nHumidity: %d\033[K\n\033[K", this->temperature, this->humidity);
}