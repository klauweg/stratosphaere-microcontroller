#include "GPSSensor.h"

void GPSSensor::configure() {
    Serial1.begin(9600, SERIAL_8N1, 34, 12);
    this->gps = new GPSData;
    this->gpgsv = new TinyGPSCustom(*this->gps, "GPRMC", 10);
}

void GPSSensor::getData() {
    while (Serial1.available() > 0) {
        int result = Serial1.read();
        Serial.write(result);
        (*this->gps).encode(result);
    }
}