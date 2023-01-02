#include "../sensor.h"

const uint8_t HIH_ADDRESS = 0x27;

class HIHData : public SensorData {
    private:
        int16_t humidity, temperature;
    public:
        HIHData(int16_t, int16_t);
        HIHData();
        void print() override;
        uint8_t convertLORA() override;
        int16_t getHumidity() const {return this->humidity;};
        int16_t getTemperature() const {return this->temperature;};
};

class HIHSensor : public Sensor<HIHData> {
    protected:
        DataResult<HIHData> getData() override;
    public:
        void configure() override;
};