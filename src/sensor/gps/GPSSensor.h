#include "../sensor.h"
#include "TinyGPS++.h"

class GPSData : public SensorData, public TinyGPSPlus {
    private:
        uint8_t visibleSatellites = 0;
        uint32_t lastUpdateTime = 0;
        uint32_t realLastUpdated = 0;
    public:
        void print() override;
        uint8_t convertLORA() override;
        void setVisibleSatellites(uint8_t count) {this->visibleSatellites = count;};
        void setLastUpdateTime(uint32_t time) {this->lastUpdateTime = time;};
        void setRealLastUpdated(uint32_t millis) {this->realLastUpdated = millis;};
        uint8_t getVisibleSatellites() const {return this->visibleSatellites;};
        uint32_t getLastUpdateTime() const {return this->lastUpdateTime;};
        uint32_t getRealLastUpdated() const {return this->realLastUpdated;};
        uint32_t getRealAge() const {return millis() - this->realLastUpdated;};
};

class GPSSensor : public Sensor<GPSData> {
    protected:
        DataResult<GPSData> getData() override;
    public:
        GPSData *gps;
        TinyGPSCustom *gpgsv;
        void configure() override;
};