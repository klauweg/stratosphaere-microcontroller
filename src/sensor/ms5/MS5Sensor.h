#include "../sensor.h"

#ifndef MS5SENSOR_H
#define MS5SENSOR_H

const uint8_t MS5_ADDRESS = 0x76;
const uint8_t CMD_ADC_READ = 0x00;
const uint8_t CMD_ADC_CONV = 0x44;
const uint8_t CMD_PROM = 0xA0;

enum measurement {	
	PRESSURE = 0x00,
	TEMPERATURE = 0x10
};

struct MS5CorrectedData {
    private:
        float pressure;
        float temperature;
    public:
        MS5CorrectedData(float, float);
        MS5CorrectedData();
        float getPressure() const {return this->pressure;};
        float getTemperature() const {return this->temperature;};
};

class MS5Data : public SensorData {
    private:
        uint32_t pressure;
        uint32_t temperature;
    public:
        MS5Data(uint32_t, uint32_t);
        MS5Data();
        void print() override {};
        void print(MS5CorrectedData corrected) const;
        uint32_t getPressure() const {return this->pressure;};
        uint32_t getTemperature() const {return this->temperature;};
};

class MS5Sensor : public Sensor<MS5Data> {
    protected:
        void measure() override;
        MS5CorrectedData correctedData;
    public:
        void configure() override;
        void correct();
        MS5CorrectedData getCorrectedData() {return this->correctedData;};
    private:
        uint16_t coefficient[8] = {0};
        int32_t getMeasurement(measurement _measurement);
};

#endif