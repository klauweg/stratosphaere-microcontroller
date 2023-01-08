#include <Arduino.h>
#include <string>
#include "module/module.h"

#ifndef SENSOR_H
#define SENSOR_H

template <typename D>
class Sensor : public Module {
  private:
    long lastTry = 0;
    virtual void measure() = 0;
  protected:
    D data;
  public:
    Sensor() = default;
    void configure() override = 0;
    D getData() {return this->data;};
    void tick() override {
        if (this->getStatus() != 0 || millis() - this->lastTry > 600000) {
            this->lastTry = millis();
            this->measure();
        }
    };
};

class SensorData {
  public:
    virtual void print() = 0;
};

#endif