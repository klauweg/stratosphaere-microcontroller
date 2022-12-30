#include <stdint.h>
#include <Arduino.h>

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
    long lastTry;
    bool functional = true;
    virtual DataResult<D> getData();
  public:
    bool isFunctional() {return this->functional;};
    void configure(); 
    D measure() {
      if (this->functional || millis() - this->lastTry > 600000) {
        this->lastTry = millis();
        DataResult<D> result = this->getData();
        Serial.println("result:");
        Serial.println(result.status);
        if (result.status != 0) { //IF VALID DATA
            this->functional = true;
            return result.data;
        } else {
            this->functional = false;
        }
      }
      return D();
    }
};

class SensorData {
  public:
    uint8_t convertLORA();
    void print();
};

#endif