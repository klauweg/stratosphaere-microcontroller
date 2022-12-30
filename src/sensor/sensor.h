#include <stdint.h>

#ifndef SENSOR_H
#define SENSOR_H


template <class D>
class Sensor {
  public:
    void configure();
    D getData();
};

class SensorData {
  public:
    uint8_t convertLORA();
    void print();
};

#endif