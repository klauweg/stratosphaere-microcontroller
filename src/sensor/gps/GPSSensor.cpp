#include "GPSSensor.h"
#include <iostream>

GPSData::GPSData(uint16_t latitude,
            uint16_t longitude,
            uint16_t altitude,
            uint8_t time,
            uint8_t satelites,
            uint8_t quality,
            uint8_t geoidal,
            uint8_t correction_age) {
    this->latitude = latitude;
    this->longitude = longitude;
    this->altitude = altitude;
    this->time = time;
    this->satelites = satelites;
    this->quality = quality;
    this->geoidal = geoidal;
    this->correction_age = correction_age;
}

void GPSSensor::configure() {
    return;
}

GPSData GPSSensor::getData() {
    return GPSData(3, 3, 3, 3, 3, 3, 3, 3);
}