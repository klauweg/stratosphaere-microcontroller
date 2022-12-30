#include "../sensor.h"
#include "TinyGPS++.h"

class GPSData : public SensorData, public TinyGPSPlus {
    private:
        uint8_t visibleSatellites;
    public:
        void print();
        uint8_t convertLORA();
        void setVisibleSatellites(uint8_t count) {this->visibleSatellites = count;};
        uint8_t getVisibleSatellites() {return this->visibleSatellites;};
};

class GPSSensor : public Sensor<GPSData> {
    protected:
        DataResult<GPSData> getData();
    public:
        GPSData *gps;
        TinyGPSCustom *gpgsv;
        void configure();
};