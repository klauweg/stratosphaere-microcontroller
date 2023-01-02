#include "../sensor.h"

const uint8_t MPU_ADDRESS = 0x68;

class MPUData : public SensorData {
    private:
        int16_t acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z, temperature;
    public:
        MPUData(int16_t, int16_t, int16_t, int16_t, int16_t, int16_t, int16_t);
        MPUData();
        void print() override;
        uint8_t convertLORA() override;
        int16_t getAccX() const {return this->acc_x;};
        int16_t getAccY() const {return this->acc_y;};
        int16_t getAccZ() const {return this->acc_z;};
        int16_t getGyroX() const {return this->gyro_x;};
        int16_t getGyroY() const {return this->gyro_y;};
        int16_t getGyroZ() const {return this->gyro_z;};
        double getTemperature() const {return this->temperature;};
};

class MPUSensor : public Sensor<MPUData> {
    protected:
        DataResult<MPUData> getData() override;
    public:
        void configure() override;
};