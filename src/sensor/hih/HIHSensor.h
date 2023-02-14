#include "../sensor.h"

#ifndef HIHSENSOR_H
#define HIHSENSOR_H

const uint8_t HIH_ADDRESS = 0x27;

struct HIHCorrectedData {
private:
    float humidity;
    float temperature;
public:
    HIHCorrectedData(float, float);
    HIHCorrectedData();
    float getHumidity() const {return this->humidity;};
    float getTemperature() const {return this->temperature;};
};

class HIHData : public SensorData {
    private:
        uint16_t humidity, temperature;
    public:
        HIHData(uint16_t, uint16_t);
        HIHData();
        void print() override {};
        void print(HIHCorrectedData corrected) const;
        uint16_t getHumidity() const {return this->humidity;};
        uint16_t getTemperature() const {return this->temperature;};
};

class HIHSensor : public Sensor<HIHData> {
    protected:
        HIHCorrectedData correctedData;
        void measure() override;
    public:
        HIHCorrectedData getCorrectedData() {return this->correctedData;};
        void correct();
        void configure() override;
};

#endif