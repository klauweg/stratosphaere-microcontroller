#include <Arduino.h>
#include <Wire.h>
#include "storage/storage.h"
#include "display/Display.h"


unsigned long lastMillis = 0;

Storage *storage = new Storage(&SD_MMC);
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

  GPSData gpsData = gpsResult.data;
  HIHData hihData = hihResult.data;
  LM75Data lm75Data = lm75Result.data;
  MPUData mpuData = mpuResult.data;
  MS5Data ms5Data = ms5Result.data;

  sdStatus = storage->storeData(gpsResult, hihResult, lm75Result, mpuResult, ms5Result);
  
  char buffer [300];
  if (millis() % 20000 < 10000) {
    sprintf(buffer, "Lora: %u SD: %u\nSensor: %u\nUSat: %u ASat: %u\n%s",
          0, sdStatus, gpsResult.status << 4 | hihResult.status << 3 | lm75Result.status << 2 | mpuResult.status << 1 | ms5Result.status, gpsData.satellites.value(), 
          gpsData.getVisibleSatellites(), storage->getFileName().c_str());
  } else {
    sprintf(buffer, "OTemp: %u\nDate: %u.%u.%u\nTime: %u.%u.%u\nLat: %2.4f\nLng: %2.4f",
      lm75Data.getTemperature(), gpsData.date.day(), gpsData.date.month(), gpsData.date.year(),
      gpsData.time.hour(), gpsData.time.minute(), gpsData.time.second(), gpsData.location.lat(), gpsData.location.lng());
  }
  display->setBuffer(buffer);

  Serial.print("\033[f");
  gpsData.print();
  hihData.print();
  lm75Data.print();
  mpuData.print();
  ms5Data.print(ms5Sensor->correct(ms5Data));
  printf("\n===[ DEBUG ]===\033[K\033[K\nLast Tick Duration: %lu\033[K\nCalculated Time in Millis: %u\033[K\n\033[K",
    millis()-lastMillis, gpsData.time.value()*10+gpsData.time.age());
}