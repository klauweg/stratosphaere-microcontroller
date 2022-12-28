#include "../sensor.h"

class MS5Data : public SensorData {
    private:
        uint16_t pressure;
    public:
        MS5Data(uint16_t);
        uint8_t convertLORA();
};

class MS5Sensor : public Sensor<MS5Data> {
    public:
        void configure();
        MS5Data getData();
};