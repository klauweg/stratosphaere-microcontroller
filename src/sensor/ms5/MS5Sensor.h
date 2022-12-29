#include "../sensor.h"

class MS5Data : public SensorData {
    public:
        int16_t pressure;
        int16_t temperature;
        MS5Data(int16_t, int16_t);
        uint8_t convertLORA();
};

class MS5Sensor : public Sensor<MS5Data> {
    public:
        void configure();
        MS5Data getData();
};