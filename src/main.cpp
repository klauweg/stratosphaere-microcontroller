#include <Arduino.h>
#include <Wire.h>
#include "storage/storage.h"
#include "display/Display.h"


long lastMillis = 0;

Storage *storage = new Storage();
Display *display = new Display();

GPSSensor *gpsSensor = new GPSSensor();
HIHSensor *hihSensor = new HIHSensor();
LM75Sensor *lm75Sensor = new LM75Sensor();
MPUSensor *mpuSensor = new MPUSensor();
MS5Sensor *ms5Sensor = new MS5Sensor();

uint8_t sdStatus = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(100000);

  display->configure();
  sdStatus = storage->configure();
  display->setSdStatus(sdStatus);

  gpsSensor->configure();
  hihSensor->configure();
  lm75Sensor->configure();
  mpuSensor->configure();
  ms5Sensor->configure();
  Serial.print("\033[2J");    // clear screen command
}

void loop() {
  lastMillis = millis();

  DataResult<GPSData> gpsResult = gpsSensor->measure();
  DataResult<HIHData> hihResult = hihSensor->measure();
  DataResult<LM75Data> lm75Result = lm75Sensor->measure();
  DataResult<MPUData> mpuResult = mpuSensor->measure();
  DataResult<MS5Data> ms5Result = ms5Sensor->measure();


  sdStatus = storage->storeData(gpsResult, hihResult, lm75Result, mpuResult, ms5Result);
  display->setSensorStatus(gpsResult.status << 4 | hihResult.status << 3 | lm75Result.status << 2 | mpuResult.status << 1 | ms5Result.status);
  display->setSdStatus(sdStatus);
  display->setUsedSatelliteStatus(gpsResult.data.satellites.value());
  display->setAvSatelliteStatus(gpsResult.data.getVisibleSatellites());
  display->setFileName(storage->getFileName());

  Serial.print("\033[f");
  Serial.println(gpsResult.status << 4 | hihResult.status << 3 | lm75Result.status << 2 | mpuResult.status << 1 | ms5Result.status);

  gpsResult.data.print();
  hihResult.data.print();
  lm75Result.data.print();
  mpuResult.data.print();
  ms5Result.data.print(ms5Sensor->correct(ms5Result.data));

  printf("\n===[ DEBUG ]===\033[K\033[K\nLast Tick Duration: %lu\033[K\nCalculated Time in Millis: %u\033[K\n\033[K",
    millis()-lastMillis, gpsResult.data.time.value()*10+gpsResult.data.time.age());
}