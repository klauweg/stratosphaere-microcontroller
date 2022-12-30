#include "GPSSensor.h"

void GPSSensor::configure() {
    this->gps = new GPSData;
    this->gpgsv = new TinyGPSCustom(*this->gps, "GPGSV", 3);
    Serial1.begin(9600, SERIAL_8N1, 34, 12);
}

DataResult<GPSData> GPSSensor::getData() {
    while (Serial1.available() > 0) {
        int result = Serial1.read();
        (*this->gps).encode(result);
    }
    if ((*this->gpgsv).isUpdated()) {
        (*this->gps).setVisibleSatellites(atoi((*this->gpgsv).value()));
    }
    return {1, (*this->gps)};
}


void GPSData::print() {
	printf("===[ GPS ]===\nLocation: lat: %f lng: %f alt: %f\nTime: %uh %um %us %ucs\nUS: %u.00\nVS: %u.00\nDate: %u.%u.%u\nSpeed: %fkmph\n\n",
		this->location.lat(), this->location.lng(), this->altitude.meters(), this->time.hour(), this->time.minute(), this->time.second(),
		this->time.centisecond(), this->satellites.value(), this->getVisibleSatellites(), this->date.day(), this->date.month(),this->date.year(), this->speed.kmph());
}