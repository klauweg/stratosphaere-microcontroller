#include "../sensor.h"

class MPUData : public SensorData {
    private:
        uint16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, temperature;
    public:
        MPUData(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
        uint8_t convertLORA();
};

class MPUSensor : public Sensor<MPUData> {
    public:
        void configure();
        MPUData getData();
};