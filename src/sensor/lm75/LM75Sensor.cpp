#include "LM75Sensor.h"
#include <Wire.h>
#include <Arduino.h>

LM75Data::LM75Data(int16_t temperature) {
    this->temperature = temperature;
}

void LM75Data::print() {
	Serial.println("===[ LM75 ]===");
  Serial.print("Temperature: ");
  Serial.println(this->temperature);
	Serial.print("\n");
}

void LM75Sensor::configure() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x01);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

LM75Data LM75Sensor::getData() {
  Wire.beginTransmission(LM75_ADDRESS);
  Wire.write(0x00);
  Wire.endTransmission(true);
  Wire.requestFrom(LM75_ADDRESS, (size_t) 2, true);
  int16_t temperature = Wire.read()<<8 | Wire.read();
  return {temperature};
}