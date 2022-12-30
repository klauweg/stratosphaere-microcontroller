#include "../sensor.h"

const uint8_t LM75_ADDRESS = 0x4f;

class LM75Data : public SensorData {
    private:
        int16_t temperature;
    public:
        LM75Data(int16_t);
        LM75Data();
        void print();
        uint8_t convertLORA();
        int16_t getTemperature() {return this->temperature;};
};

class LM75Sensor : public Sensor<LM75Data> {
    protected:
        DataResult<LM75Data> getData();
    public:
        void configure();
};