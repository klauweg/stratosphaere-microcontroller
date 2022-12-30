#include "../sensor.h"

const uint8_t LM75_ADDRESS = 0x4f;

class LM75Data : public SensorData {
    public:
        int16_t temperature;
        LM75Data(int16_t);
        void print();
        uint8_t convertLORA();
};

class LM75Sensor : public Sensor<LM75Data> {
    public:
        void configure();
        LM75Data getData();
};