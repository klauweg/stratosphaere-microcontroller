#include "../sensor.h"

const uint8_t MS5_ADDRESS = 0x76;
const uint8_t CMD_ADC_READ = 0x00;
const uint8_t CMD_ADC_CONV = 0x48;
const uint8_t CMD_PROM = 0xA0;

enum measurement {	
	PRESSURE = 0x00,
	TEMPERATURE = 0x10
};

class MS5Data : public SensorData {
    public:
        int32_t pressure;
        int32_t temperature;
        MS5Data(int32_t, int32_t);
        uint8_t convertLORA();
};

struct MS5CorrectedData {
    float pressure;
    float temperature;
};

class MS5Sensor : public Sensor<MS5Data> {
    public:
        uint16_t coefficient[8];
        void configure();
        MS5Data getData();
        MS5CorrectedData correct(MS5Data data);
    private:
        int32_t getMeasurement(measurement _measurement);
};