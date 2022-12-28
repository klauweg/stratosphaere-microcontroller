#include "MPUSensor.h"
#include <iostream>

void MPUSensor::configure() {
    std::cout << "7\n";
    return;
}

MPUData::MPUData(uint16_t acc_x, 
            uint16_t acc_y,
            uint16_t acc_z,
            uint16_t gyro_x,
            uint16_t gyro_y,
            uint16_t gyro_z,
            uint16_t temperature) {
    this->acc_x = acc_x;
    this->acc_y = acc_y;
    this->acc_z = acc_z;
    this->gyro_x = gyro_x;
    this->gyro_y = gyro_y;
    this->gyro_z = gyro_z;
    this->temperature = temperature;
}

MPUData MPUSensor::getData() {
    return MPUData(3, 3, 3, 3, 3, 3, 3);
}