#include "../sensor.h"

#ifndef LM75SENSOR_H
#define LM75SENSOR_H

const uint8_t LM75_ADDRESS = 0x4f;

struct LM75CorrectedData {
private:
    float temperature;
public:
    explicit LM75CorrectedData(float);
    LM75CorrectedData();
    float getTemperature() const {return this->temperature;};
};

class LM75Data : public SensorData {
    private:
        int16_t temperature;
    public:
        explicit LM75Data(int16_t);
        LM75Data();
        void print() override {};
        void print(LM75CorrectedData corrected) const;
        int16_t getTemperature() const {return this->temperature;};
};

class LM75Sensor : public Sensor<LM75Data> {
    protected:
        void measure() override;
        LM75CorrectedData correctedData;
    public:
        LM75CorrectedData getCorrectedData() {return this->correctedData;};
        void correct();
        void configure() override;
};

#endif