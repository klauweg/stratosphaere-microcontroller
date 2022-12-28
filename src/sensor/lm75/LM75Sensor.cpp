#include "LM75Sensor.h"
#include <iostream>

void LM75Sensor::configure() {
    std::cout << "6\n";
    return;
}

LM75Data::LM75Data(uint16_t temperature) {
    this->temperature = temperature;
}

LM75Data LM75Sensor::getData() {
    return LM75Data(3);
}