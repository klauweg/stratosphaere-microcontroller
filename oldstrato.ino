/*#include <Wire.h>
#include "sensordata.h"

const uint8_t MPU_ADDRESS = 0x68;



void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);

  //MPU Sensor
  Wire.beginTransmission(MPU_ADDRESS); // I2C Transmission
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  
}

void loop() {
  MpuData mpuData = readMPUData();
  
  Wire.beginTransmission(0x76);
  // Start with register 0x00
  Wire.write(0x00);
  Wire.endTransmission(false);

  Wire.requestFrom((uint16_t) 0x76, (size_t) 3, true);
  int16_t pressure = Wire.read();
  int8_t temperature = Wire.read()<<8 | Wire.read();
  //Serial.println(pressure);
  Serial.println(temperature);
  
  delay(100);
}
*/