#include "sensor/hih/HIHSensor.h"
#include <iostream>
using namespace std;

//..\arduino-cli.exe compile strato.ino --fqbn esp32:esp32:t-beam

void setup() {
  std::cout << "test\n";
  HIHSensor *hihSensor = new HIHSensor();
  hihSensor->configure();
  std::cout << hihSensor->getData().test;
  std::cout << "test2\n";
}

void loop() {

}