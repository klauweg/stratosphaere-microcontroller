#include "../sensor.h"

class LM75Data : public SensorData {
    private:
        uint16_t temperature;
    public:
        LM75Data(uint16_t);
        uint8_t convertLORA();
};

class LM75Sensor : public Sensor<LM75Data> {
    public:
        void configure();
        LM75Data getData();
};