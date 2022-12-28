/*MpuData getMPUData() {
  Wire.beginTransmission(MPU_ADDRESS);
  // Start with register 0x3B
  Wire.write(0x3B);
  Wire.endTransmission(false);

  
    Sensor automatically sends next registers when requesting more data
    3B = ACCEL_XOUT_H    3C = ACCEL_XOUT_L    3D = ACCEL_YOUT_H    3E = ACCEL_YOUT_L    3F = ACCEL_ZOUT_H    40 = ACCEL_ZOUT_L
    41 = TEMP_OUT_H    42 = TEMP_OUT_L
    43 = GYRO_XOUT_H    44 = GYRO_XOUT_L    45 = GYRO_YOUT_H    46 = GYRO_YOUT_L    47 = GYRO_ZOUT_H    48 = GYRO_ZOUT_L
  
  Wire.requestFrom(MPU_ADDRESS, (size_t) 7*2, true);
  int16_t acc_x = Wire.read()<<8 | Wire.read();
  int16_t acc_y = Wire.read()<<8 | Wire.read();
  int16_t acc_z = Wire.read()<<8 | Wire.read();
  int16_t temperature = Wire.read()<<8 | Wire.read();
  int16_t gyro_x = Wire.read()<<8 | Wire.read();
  int16_t gyro_y = Wire.read()<<8 | Wire.read();
  int16_t gyro_z = Wire.read()<<8 | Wire.read();
  return {temperature/340.00+36.53, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z};
}*/