#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/ms5/MS5Sensor.h"
#include <iostream>
#include <Arduino.h>
#include <Wire.h>
#include "SD_MMC.h"

long lastMillis = 0;

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
  Serial.println("Last Loop Duration: ");
  Serial.println(millis()-lastMillis);
  lastMillis = millis();
  //HIHData hihData = hihSensor->getData();
  GPSData gpsData = gpsSensor->getData();
  //LM75Data lm75Data = lm75Sensor->getData();
  MPUData mpuData = mpuSensor->getData();
  MS5Data ms5Data = ms5Sensor->getData();
  Serial.println("-----------------");
    if (gpsData.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gpsData.time.hour());
    Serial.print(":");
    if (gpsData.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gpsData.time.minute());
    Serial.print(":");
    if (gpsData.time.second() < 10) Serial.print(F("0"));
    Serial.print(gpsData.time.second());
    Serial.print(".");
    if (gpsData.time.centisecond() < 10) Serial.print(F("0"));
    Serial.println(gpsData.time.centisecond());
  Serial.println(gpsData.location.lat());
  Serial.println(gpsData.location.lng());
  Serial.println("-----------------");
  //Serial.println(mpuData.temperature);
  //Serial.println(ms5Data.pressure);
  //Serial.println(ms5Data.temperature);
  //Serial.println(ms5Sensor->correct(ms5Data).pressure);
  //Serial.println(ms5Sensor->correct(ms5Data).temperature);
  delay(10000);
}