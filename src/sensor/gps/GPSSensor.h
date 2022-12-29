#include "../sensor.h"

class GPSData : public SensorData {
    public:
        uint16_t latitude, longitude, altitude;
        uint8_t time, satelites, quality, geoidal, correction_age;
        GPSData(uint16_t, uint16_t, uint16_t, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
        uint8_t convertLORA();
};

class GPSSensor : public Sensor<GPSData> {
    public:
        void configure();
        GPSData getData();
};