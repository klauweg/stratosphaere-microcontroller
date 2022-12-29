#include "../sensor.h"

class HIHData : public SensorData {
    public:
        int16_t humidity, temperature;
        HIHData(int16_t, int16_t);
        uint8_t convertLORA();
};

class HIHSensor : public Sensor<HIHData> {
    public:
        void configure();
        HIHData getData();
};