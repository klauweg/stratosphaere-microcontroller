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
        void print(MS5CorrectedData corrected);
        uint8_t convertLORA();
        int32_t getPressure() {return this->pressure;};
        int32_t getTemperature() {return this->temperature;};
};

class MS5Sensor : public Sensor<MS5Data> {
    protected:
        DataResult<MS5Data> getData();
    public:
        void configure();
        void print();
        MS5CorrectedData correct(MS5Data data);
        uint16_t getCoefficient() {return *(this->coefficient);};
    private:
        uint16_t coefficient[8];
        DataResult<int32_t> getMeasurement(measurement _measurement);
};