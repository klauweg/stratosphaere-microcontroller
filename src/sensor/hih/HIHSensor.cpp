#include "HIHSensor.h"
#include <Arduino.h>
#include <Wire.h>

void HIHSensor::configure() {
}

HIHData::HIHData(int16_t humidity, int16_t temperature) {
    this->humidity = humidity;
    this->temperature = temperature;
}

HIHData HIHSensor::getData() {
  Wire.beginTransmission(HIH_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(true);
  Wire.requestFrom(HIH_ADDRESS, (size_t) 2*2, true);
  int16_t humidity = Wire.read()<<8 | Wire.read();
  int16_t temperature = Wire.read()<<8 | Wire.read();
  return {humidity, temperature};
}

void HIHData::print() {
	Serial.println("===[ HIH ]===");
  Serial.print("Temperature: ");
  Serial.println(this->temperature);
  Serial.print("Humidity: ");
  Serial.println(this->humidity);
	Serial.print("\n");
}