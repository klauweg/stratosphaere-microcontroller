#include "sensor/hih/HIHSensor.h"
#include <sensor.h>

//..\arduino-cli.exe compile strato.ino --fqbn esp32:esp32:t-beam

void setup() {
    HIHSensor *hihSensor = new HIHSensor();
    hihSensor->configure();
}

void loop() {

}