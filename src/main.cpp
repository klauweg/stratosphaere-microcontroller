#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/ms5/MS5Sensor.h"
#include <iostream>
#include <Arduino.h>
#include <Wire.h>
#include "SD_MMC.h"

//..\arduino-cli.exe compile strato.ino --fqbn esp32:esp32:t-beam
HIHSensor *hihSensor = new HIHSensor();
GPSSensor *gpsSensor = new GPSSensor();
LM75Sensor *lm75Sensor = new LM75Sensor();
MPUSensor *mpuSensor = new MPUSensor();
MS5Sensor *ms5Sensor = new MS5Sensor();

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);

  hihSensor->configure();
  gpsSensor->configure();
  lm75Sensor->configure();
  mpuSensor->configure();
  ms5Sensor->configure();
}

void loop() {
  delay(10000);
  //HIHData hihData = hihSensor->getData();
  //GPSData gpsData = gpsSensor->getData();
  //LM75Data lm75Data = lm75Sensor->getData();
  MPUData mpuData = mpuSensor->getData();
  MS5Data ms5Data = ms5Sensor->getData();
  Serial.println("-----------------");
  Serial.println(mpuData.temperature);
  Serial.println(ms5Data.pressure);
  Serial.println(ms5Data.temperature);
}