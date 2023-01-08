#include "../sensor.h"

#ifndef LM75SENSOR_H
#define LM75SENSOR_H

const uint8_t LM75_ADDRESS = 0x4f;

class LM75Data : public SensorData {
    private:
        int16_t temperature;
    public:
        explicit LM75Data(int16_t);
        LM75Data();
        void print() override;
        int16_t getTemperature() const {return this->temperature;};
};

class LM75Sensor : public Sensor<LM75Data> {
    protected:
        DataResult<LM75Data> getData() override;
    public:
        void configure() override;
};

#endif