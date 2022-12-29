#include "../sensor.h"
#include "TinyGPS++.h"

class GPSData : public SensorData, public TinyGPSPlus {
    public:
        uint8_t convertLORA();
};

class GPSSensor : public Sensor<GPSData> {
    public:
        GPSSensor();
        void configure();
        GPSData getData();
};