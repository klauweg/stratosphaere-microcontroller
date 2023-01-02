#include "../sensor.h"
#include "TinyGPS++.h"

class GPSData : public SensorData, public TinyGPSPlus {
    private:
        uint8_t visibleSatellites;
        uint32_t lastUpdateTime;
        uint32_t realLastUpdated;
    public:
        void print();
        uint8_t convertLORA();
        void setVisibleSatellites(uint8_t count) {this->visibleSatellites = count;};
        void setLastUpdateTime(uint32_t time) {this->lastUpdateTime = time;};
        void setRealLastUpdated(uint32_t millis) {this->realLastUpdated = millis;};
        uint8_t getVisibleSatellites() {return this->visibleSatellites;};
        uint32_t getLastUpdateTime() {return this->lastUpdateTime;};
        uint32_t getRealLastUpdated() {return this->realLastUpdated;};
        uint32_t getRealAge() {return millis() - this->realLastUpdated;};
};

class GPSSensor : public Sensor<GPSData> {
    protected:
        DataResult<GPSData> getData();
    public:
        GPSData *gps;
        TinyGPSCustom *gpgsv;
        void configure();
};