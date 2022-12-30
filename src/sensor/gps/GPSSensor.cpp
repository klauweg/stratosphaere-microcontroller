#include "GPSSensor.h"

void GPSSensor::configure() {
    this->gps = new GPSData;
    this->gpgsv = new TinyGPSCustom(*this->gps, "GPGSV", 3);
    Serial1.begin(9600, SERIAL_8N1, 34, 12);
}

GPSData GPSSensor::getData() {
    while (Serial1.available() > 0) {
        int result = Serial1.read();
        (*this->gps).encode(result);
    }
    if ((*this->gpgsv).isUpdated()) {
		Serial.println("Sateilitesacadawdwad:");
		Serial.println((*this->gpgsv).value());
        (*this->gps).visibleSatelites = atoi((*this->gpgsv).value());
    }
    return (*this->gps);
}


void GPSData::print() {
	Serial.println("===[ GPS ]===");
	Serial.print("Location: ");
	Serial.print("lat: ");
	Serial.print(this->location.lat(), 9);
	Serial.print(" lng: ");
	Serial.print(this->location.lng(), 9);
	Serial.print(" alt: ");
	Serial.println(this->altitude.meters());
	Serial.print("Time: ");
	Serial.print(this->time.hour());
	Serial.print("h ");
	Serial.print(this->time.minute());
	Serial.print("m ");
	Serial.print(this->time.second());
	Serial.print("s ");
	Serial.print(this->time.centisecond());
	Serial.println("cs ");
	Serial.print("Used Satelites: ");
	Serial.print(this->satellites.value());
	Serial.println(".00");
	Serial.print("Visible Satelites: ");
	Serial.print(this->visibleSatelites);
	Serial.println(".00");
	Serial.print("Date: ");
	Serial.print(this->date.day());
	Serial.print(".");
	Serial.print(this->date.month());
	Serial.print(".");
	Serial.println(this->date.year());
	Serial.print("Speed: ");
    Serial.print(this->speed.kmph());
    Serial.println("kmph");
	Serial.print("\n");
}