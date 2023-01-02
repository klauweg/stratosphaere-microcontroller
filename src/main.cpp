#include <Arduino.h>
#include <Wire.h>
#include "storage/storage.h"


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

  Storage::configure();

  hihSensor->configure();
  gpsSensor->configure();
  lm75Sensor->configure();
  mpuSensor->configure();
  ms5Sensor->configure();
  Serial.print("\033[2J");    // clear screen command
}

void loop() {
  lastMillis = millis();

  DataResult<HIHData> hihResult = hihSensor->measure();
  DataResult<GPSData> gpsResult = gpsSensor->measure();
  DataResult<LM75Data> lm75Result = lm75Sensor->measure();
  DataResult<MPUData> mpuResult = mpuSensor->measure();
  DataResult<MS5Data> ms5Result = ms5Sensor->measure();

  Serial.print("\033[f");

  gpsResult.data.print();
  hihResult.data.print();
  lm75Result.data.print();
  mpuResult.data.print();
  ms5Result.data.print(ms5Sensor->correct(ms5Result.data));

  Storage::storeData(gpsResult, hihResult, lm75Result, mpuResult, ms5Result);

  printf("\n===[ DEBUG ]===\033[K\033[K\nLast Tick Duration: %lu\033[K\nCalculated Time in Millis: %u\033[K\n\033[K",
    millis()-lastMillis, gpsResult.data.time.value()*10+gpsResult.data.time.age());
}