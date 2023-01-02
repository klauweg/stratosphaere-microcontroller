#include "../sensor.h"

const uint8_t MPU_ADDRESS = 0x68;

class MPUData : public SensorData {
    private:
        int16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, temperature;
    public:
        MPUData(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        MPUData();
        void print();
        uint8_t convertLORA();
        int16_t getAccX() {return this->acc_x;};
        int16_t getAccY() {return this->acc_y;};
        int16_t getAccZ() {return this->acc_z;};
        int16_t getGyroX() {return this->gyro_x;};
        int16_t getGyroY() {return this->gyro_y;};
        int16_t getGyroZ() {return this->gyro_z;};
        double getTemperature() {return this->temperature;};
};

class MPUSensor : public Sensor<MPUData> {
    protected:
        DataResult<MPUData> getData();
    public:
        void configure();
};