#include "SD_MMC.h"
#include "sensor/ms5/MS5Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"

#ifndef STORAGE_H
#define STORAGE_H

class Storage : public Module {
    public:
        explicit Storage(fs::SDMMCFS *fs, GPSSensor*, LM75Sensor*, HIHSensor*, MPUSensor*, MS5Sensor*);
        std::string getFileName() {return this->fileName;};
        void tick() override;
        void configure() override;
    private:
        GPSSensor *gpsSensor;
        LM75Sensor *lm75Sensor;
        HIHSensor *hihSensor;
        MPUSensor *mpuSensor;
        MS5Sensor *ms5Sensor;
        fs::SDMMCFS *fs;
        File file;
        std::string fileName = "unknown.png";
        unsigned long lastMountTry = 0;
        bool mountFailed = false;
        int32_t getFileCount();
};

#endif