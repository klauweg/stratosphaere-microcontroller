#include "SD_MMC.h"
#include "sensor/ms5/MS5Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include <string>

class Storage {
    public:
        explicit Storage(fs::SDMMCFS *fs);
        uint8_t configure();
        std::string getFileName() {return this->fileName;};
        uint8_t storeData(const DataResult<GPSData>&, const DataResult<HIHData>&, const DataResult<LM75Data>&, const DataResult<MPUData>&,const DataResult<MS5Data>&);
    private:
        fs::SDMMCFS *fs;
        std::string fileName = "unknown.png";
        unsigned long lastMountTry = 0;
        bool mountFailed = false;
        int32_t getFileCount();
};