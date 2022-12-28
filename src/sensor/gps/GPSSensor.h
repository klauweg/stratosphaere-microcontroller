#include "../sensor.h"

class GPSData : public SensorData {
    private:
        uint16_t latitude, longitude, altitude;
        uint8_t time, satelites, quality, geoidal, correction_age;
    public:
        GPSData(uint16_t, uint16_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
        uint8_t convertLORA();
};

class GPSSensor : public Sensor<GPSData> {
    public:
        void configure();
        GPSData getData();
};