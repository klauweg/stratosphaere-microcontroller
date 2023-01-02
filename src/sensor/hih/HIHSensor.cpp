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

DataResult<HIHData> HIHSensor::getData() {
  Wire.beginTransmission(HIH_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(true);
  uint8_t status = Wire.requestFrom(HIH_ADDRESS, (size_t) 2*2, true);
  int16_t humidity = Wire.read()<<8 | Wire.read();
  int16_t temperature = Wire.read()<<8 | Wire.read();
  return {static_cast<uint8_t>(status == 0 ? 0 : 1), HIHData(humidity, temperature)};
}

void HIHData::print() {
	printf("\n===[ HIH ]===\033[K\nTemperature: %d\033[K\nHumidity: %d\033[K\n\033[K", this->temperature, this->humidity);
}

uint8_t HIHData::convertLORA() {
    return 0;
}
