#include "../sensor.h"

const uint8_t HIH_ADDRESS = 0x27;

class HIHData : public SensorData {
    public:
        int16_t humidity, temperature;
        HIHData(int16_t, int16_t);
        void print();
        uint8_t convertLORA();
};

class HIHSensor : public Sensor<HIHData> {
    public:
        void configure();
        HIHData getData();
};