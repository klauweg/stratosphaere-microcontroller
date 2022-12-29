#include "GPSSensor.h"

void GPSSensor::configure() {
}

GPSSensor::GPSSensor() {
    Serial1.begin(9600, SERIAL_8N1, 34, 12);
}

GPSData GPSSensor::getData() {
    GPSData gps = GPSData();
    while (Serial1.available() > 0) {
        gps.encode(Serial1.read());
    }
    return GPSData(gps);
}