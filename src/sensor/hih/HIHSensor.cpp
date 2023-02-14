#include "HIHSensor.h"
#include <Wire.h>

void HIHSensor::configure() {
}

HIHData::HIHData(uint16_t humidity, uint16_t temperature) {
    this->humidity = humidity;
    this->temperature = temperature;
}
HIHData::HIHData() {
    this->humidity = 0;
    this->temperature = 0;
}
HIHCorrectedData::HIHCorrectedData(float temperature, float humidity) {
    this->humidity = humidity;
    this->temperature = temperature;
}
HIHCorrectedData::HIHCorrectedData() {
    this->humidity = 0;
    this->temperature = 0;
}

void HIHSensor::measure() {
    this->status = Wire.requestFrom(HIH_ADDRESS, (size_t) 2*2, true);
    uint16_t humidity = Wire.read()<<8 | Wire.read();
    humidity = humidity & 0x3fff;
    uint16_t temperature = Wire.read()<<8 | Wire.read();
    temperature = temperature >> 2;
    this->data = HIHData(humidity, temperature);
    Wire.beginTransmission(HIH_ADDRESS);
    Wire.endTransmission(true);
    this->correct();
}

void HIHSensor::correct() {
    uint16_t temperature = this->getData().getTemperature();
    uint16_t humidity = this->getData().getHumidity();
    this->correctedData = HIHCorrectedData{(temperature/16382.0f)*165.0f-40.0f, (humidity/(16382.0f))*100};
}

void HIHData::print(HIHCorrectedData corrected) const {
	printf("\n===[ HIH ]===\033[K\nTemperature: %d\033[K\nHumidity: %d\033[K\n\033[K\nTemperature: %f\033[K\nHumidity: %f\033[K\n\033[K",
           this->temperature, this->humidity, corrected.getTemperature(), corrected.getHumidity());
}