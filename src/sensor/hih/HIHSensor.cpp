#include "HIHSensor.h"
#include <iostream>

void HIHSensor::configure() {
    std::cout << "5\n";
    return;
}

HIHData::HIHData(uint16_t humidity, uint16_t temperature) {
    this->humidity = humidity;
    this->temperature = temperature;
}

HIHData HIHSensor::getData() {
    return HIHData(1, 2);
}