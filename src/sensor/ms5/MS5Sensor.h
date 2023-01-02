#include "../sensor.h"

const uint8_t MS5_ADDRESS = 0x76;
const uint8_t CMD_ADC_READ = 0x00;
const uint8_t CMD_ADC_CONV = 0x48;
const uint8_t CMD_PROM = 0xA0;

enum measurement {	
	PRESSURE = 0x00,
	TEMPERATURE = 0x10
};

struct MS5CorrectedData {
    float pressure;
    float temperature;
};

class MS5Data : public SensorData {
    private:
        int32_t pressure;
        int32_t temperature;
    public:
        MS5Data(int32_t, int32_t);
        MS5Data();
        void print() override {};
        void print(MS5CorrectedData corrected) const;
        uint8_t convertLORA() override;
        int32_t getPressure() const {return this->pressure;};
        int32_t getTemperature() const {return this->temperature;};
};

class MS5Sensor : public Sensor<MS5Data> {
    protected:
        DataResult<MS5Data> getData() override;
    public:
        void configure() override;
        MS5CorrectedData correct(const MS5Data& data);
    private:
        uint16_t coefficient[8] = {0};
        static DataResult<int32_t> getMeasurement(measurement _measurement);
};