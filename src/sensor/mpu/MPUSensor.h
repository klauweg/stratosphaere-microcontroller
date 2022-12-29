#include "../sensor.h"

class MPUData : public SensorData {
    public:
        int16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;
        double temperature;
        MPUData(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, double);
        uint8_t convertLORA();
};

class MPUSensor : public Sensor<MPUData> {
    public:
        void configure();
        MPUData getData();
};