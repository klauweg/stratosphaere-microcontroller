#include "SD_MMC.h"
#include "sensor/ms5/MS5Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"

namespace Storage {
    void configure();
    void storeData(DataResult<GPSData>, DataResult<HIHData>, DataResult<LM75Data>, DataResult<MPUData>,DataResult<MS5Data>);
    uint32_t getFileCount();
}