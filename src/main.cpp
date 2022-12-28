#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/ms5/MS5Sensor.h"
#include <iostream>
using namespace std;

//..\arduino-cli.exe compile strato.ino --fqbn esp32:esp32:t-beam

void setup() {
  HIHSensor *hihSensor = new HIHSensor();
  hihSensor->configure();
  GPSSensor *gpsSensor = new GPSSensor();
  gpsSensor->configure();
  LM75Sensor *lm75Sensor = new LM75Sensor();
  lm75Sensor->configure();
  MPUSensor *mpuSensor = new MPUSensor();
  mpuSensor->configure();
  MS5Sensor *ms5Sensor = new MS5Sensor();
  ms5Sensor->configure();
}

void loop() {

}