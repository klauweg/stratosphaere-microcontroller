#include "GPSSensor.h"

void GPSSensor::configure() {
    this->gps = new GPSData;
    this->gpgsv = new TinyGPSCustom(*this->gps, "GPGSV", 2);
    Serial1.begin(9600, SERIAL_8N1, 34, 12);
}

GPSData GPSSensor::getData() {
    while (Serial1.available() > 0) {
        int result = Serial1.read();
        Serial.write(result);
        (*this->gps).encode(result);
    }
    if ((*this->gpgsv).isUpdated()) {
        Serial.println("Number Of Sensors is ");
        Serial.println((*this->gpgsv).value());
    }
    return (*this->gps);
}