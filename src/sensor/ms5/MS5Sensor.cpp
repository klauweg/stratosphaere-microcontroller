#include "MS5Sensor.h"
#include <iostream>

void MS5Sensor::configure() {
    std::cout << "8\n";
    return;
}

MS5Data::MS5Data(uint16_t pressure) {
    this->pressure = pressure;
}

MS5Data MS5Sensor::getData() {
    return MS5Data(10);
}