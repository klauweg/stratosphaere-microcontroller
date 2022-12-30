#include "sensor/ms5/MS5Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include <Arduino.h>
#include <Wire.h>

long lastMillis = 0;

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
  Serial.print("\033[2J");    // clear screen command
}

void loop() {
  lastMillis = millis();

  HIHData hihData = hihSensor->measure();
  GPSData gpsData = gpsSensor->measure();
  LM75Data lm75Data = lm75Sensor->measure();
  MPUData mpuData = mpuSensor->measure();
  MS5Data ms5Data = ms5Sensor->measure();

  //Serial.print("\033[f");

  //hihData.print();
  //gpsData.print();
  //lm75Data.print();
  //mpuData.print();
  //ms5Data.print(ms5Sensor->correct(ms5Data));

  Serial.println("===[ DEBUG ]===");
  Serial.print("Last Tick Duration: ");
  Serial.println(millis()-lastMillis);
  Serial.print("\n");
  delay(10000);
}