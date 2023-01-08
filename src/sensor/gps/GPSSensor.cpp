#include "GPSSensor.h"

void GPSSensor::configure() {
    this->data = GPSData{};
    this->gpgsv = new TinyGPSCustom(this->data, "GPGSV", 3);
    Serial1.begin(9600, SERIAL_8N1, 34, 12);
}

void GPSSensor::measure() {
    while (Serial1.available() > 0) {
        int result = Serial1.read();
        this->data.encode(result);
    }
    //We have to write our own age method and include the value comparison
    if (this->data.time.isUpdated() && this->data.time.value() != this->data.getLastUpdateTime()) {
        this->data.setRealLastUpdated(millis());
        this->data.setLastUpdateTime(this->data.time.value());
    }
    if ((*this->gpgsv).isUpdated()) {
        this->data.setVisibleSatellites(atoi((*this->gpgsv).value()));
    }
    this->status = 1;
}

void GPSData::print() {
	printf("\n===[ GPS ]===\033[K\nLocation: lat: %f lng: %f alt: %f\033[K\nTime: %uh %um %us %ucs\033[K\nUS: %u.00\033[K\nVS: %u.00\033[K\nDate: %u.%u.%u\033[K\nSpeed: %fkmph\033[K\n\033[K",
		this->location.lat(), this->location.lng(), this->altitude.meters(), this->time.hour(), this->time.minute(), this->time.second(),
		this->time.centisecond(), this->satellites.value(), this->getVisibleSatellites(), this->date.day(), this->date.month(),this->date.year(), this->speed.kmph());
}
