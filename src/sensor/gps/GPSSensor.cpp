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
    //We have to write our own age method and include the value comparison
    if ((*this->gps).time.isUpdated() && (*this->gps).time.value() != (*this->gps).getLastUpdateTime()) {
        (*this->gps).setRealLastUpdated(millis());
        (*this->gps).setLastUpdateTime((*this->gps).time.value());
    }
    if ((*this->gpgsv).isUpdated()) {
        (*this->gps).setVisibleSatellites(atoi((*this->gpgsv).value()));
    }
    return {1, (*this->gps)};
}


void GPSData::print() {
	printf("\n===[ GPS ]===\033[K\nLocation: lat: %f lng: %f alt: %f\033[K\nTime: %uh %um %us %ucs\033[K\nUS: %u.00\033[K\nVS: %u.00\033[K\nDate: %u.%u.%u\033[K\nSpeed: %fkmph\033[K\n\033[K",
		this->location.lat(), this->location.lng(), this->altitude.meters(), this->time.hour(), this->time.minute(), this->time.second(),
		this->time.centisecond(), this->satellites.value(), this->getVisibleSatellites(), this->date.day(), this->date.month(),this->date.year(), this->speed.kmph());
}