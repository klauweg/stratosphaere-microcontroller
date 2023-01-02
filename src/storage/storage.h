#include "SD_MMC.h"
#include "sensor/ms5/MS5Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include <string>

class Storage {
    public:
        uint8_t configure();
        std::string getFileName() {return this->fileName;};
        uint8_t storeData(DataResult<GPSData>, DataResult<HIHData>, DataResult<LM75Data>, DataResult<MPUData>,DataResult<MS5Data>);
    private:
        std::string fileName = "";
        long lastMountTry;
        bool mountFailed;
        int32_t getFileCount();
};