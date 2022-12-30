#include "../sensor.h"

const uint8_t HIH_ADDRESS = 0x27;

class HIHData : public SensorData {
    private:
        int16_t humidity, temperature;
    public:
        HIHData(int16_t, int16_t);
        HIHData();
        void print();
        uint8_t convertLORA();
        int16_t getHumidity() {return this->humidity;};
        int16_t getTemperature() {return this->temperature;};
};

class HIHSensor : public Sensor<HIHData> {
    protected:
        DataResult<HIHData> getData();
    public:
        void configure();
};