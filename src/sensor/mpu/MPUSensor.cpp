#include "MPUSensor.h"
#include <Wire.h>

MPUData::MPUData(int16_t acc_x, int16_t acc_y, int16_t acc_z,
            int16_t gyro_x, int16_t gyro_y, int16_t gyro_z,
            int16_t temperature = 0) {
    this->acc_x = acc_x; this->acc_y = acc_y; this->acc_z = acc_z;
    this->gyro_x = gyro_x; this->gyro_y = gyro_y; this->gyro_z = gyro_z;
    this->temperature = temperature;
}

MPUData::MPUData() {
    this->acc_x = 0; this->acc_y = 0; this->acc_z = 0; 
    this->gyro_x = 0; this->gyro_y = 0; this->gyro_z = 0;
    this->temperature = 0;
}

void MPUSensor::configure() {
  Wire.beginTransmission(MPU_ADDRESS); // I2C Transmission
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
}

DataResult<MPUData> MPUSensor::getData() {
  Wire.beginTransmission(MPU_ADDRESS);
  Wire.write(0x3B);
  Wire.endTransmission(true);
  uint8_t status = Wire.requestFrom(MPU_ADDRESS, (size_t) 7*2, true);
  int16_t acc_x = Wire.read()<<8 | Wire.read();
  int16_t acc_y = Wire.read()<<8 | Wire.read();
  int16_t acc_z = Wire.read()<<8 | Wire.read();
  int16_t temperature = Wire.read()<<8 | Wire.read();
  int16_t gyro_x = Wire.read()<<8 | Wire.read();
  int16_t gyro_y = Wire.read()<<8 | Wire.read();
  int16_t gyro_z = Wire.read()<<8 | Wire.read();
  return {static_cast<uint8_t>(status == 0 ? 0 : 1), MPUData(acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, temperature)};
}

void MPUData::print() {
	printf("\n===[ MPU ]===\033[K\nAcceleration: x: %d y: %d z: %d\033[K\nGyroscope: x: %d y: %d z: %d\033[K\nTemperature: %d\033[K\n\033[K",
    this->acc_x, this->acc_y, this->acc_z, this->gyro_x, this->gyro_y, this->gyro_z, this->temperature);
}