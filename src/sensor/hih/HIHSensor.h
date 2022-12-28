#include "../sensor.h"

class HIHData : public SensorData {
    private:
        uint16_t humidity, temperature;
    public:
        HIHData(uint16_t, uint16_t);
        uint8_t convertLORA();
};

class HIHSensor : public Sensor<HIHData> {
    public:
        void configure();
        HIHData getData();
};