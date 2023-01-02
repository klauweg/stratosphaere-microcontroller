#include <cstdint>
#include <Arduino.h>
#include <string>

#ifndef SENSOR_H
#define SENSOR_H


template <class D>
struct DataResult {
  uint8_t status;
  D data;
};

template <typename D>
class Sensor {
  private:
    long lastTry = 0;
    bool functional = true;
    virtual DataResult<D> getData() = 0;
  public:
    bool isFunctional() {return this->functional;};
    virtual void configure() = 0; 
    DataResult<D> measure() {
      if (this->functional || millis() - this->lastTry > 600000) {
        this->lastTry = millis();
        DataResult<D> result = this->getData();
        if (result.status != 0) { //IF VALID DATA
            this->functional = true;
            return result;
        } else {
            this->functional = false;
        }
      }
      return {0, D()};
    }
};

class SensorData {
  public:
    virtual uint8_t convertLORA() = 0;
    virtual void print() = 0;
};


#endif