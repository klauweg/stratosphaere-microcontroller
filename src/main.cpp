#include "sensor/gps/GPSSensor.h"
#include <Arduino.h>

long lastMillis = 0;

GPSSensor *gpsSensor = new GPSSensor();

void setup() {
  Serial.begin(115200);
  gpsSensor->configure();
}

void loop() {
  Serial.println("Last Loop Duration: ");
  Serial.println(millis()-lastMillis);
  lastMillis = millis();
  //HIHData hihData = hihSensor->getData();
  gpsSensor->getData();
  delay(10000);
}