#include "TinyGPS++.h"

class GPSData : public TinyGPSPlus {
};

class GPSSensor {
    public:
        GPSData *gps;
        TinyGPSCustom *gpgsv;
        void configure();
        void getData();
};