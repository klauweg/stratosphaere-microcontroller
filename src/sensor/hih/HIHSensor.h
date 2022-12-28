#include "../sensor.h"

struct HIHData {
    int test;
    int test2;
};

class HIHSensor : public Sensor<HIHData> {
    public:
        void configure();
        HIHData getData();
};