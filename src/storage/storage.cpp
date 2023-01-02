#include "storage.h"

Storage::Storage(fs::SDMMCFS *fileSystem) {
    this->fs = fileSystem;
}

uint8_t Storage::configure() {
    this->lastMountTry = millis();
    this->fs->end();
    if(!this->fs->begin("/sdcard", true)){
        return 2;
    }
    int32_t fileCount = Storage::getFileCount();
    if (fileCount == -1) {
        return 3;
    }
    this->fileName = "/stratodata-" + std::to_string(fileCount) + ".txt";
    this->file = this->fs->open("/stratodata-6.txt", FILE_APPEND);
    if(!file){
        this->mountFailed = true;
        return 4;
    }
    this->mountFailed = false;
    return 1;
}

uint8_t Storage::storeData(const DataResult<GPSData>& gpsResult,
        const DataResult<HIHData>& hihResult,
        const DataResult<LM75Data>& lm75Result,
        const DataResult<MPUData>& mpuResult,
        const DataResult<MS5Data>& ms5Result) {
    GPSData gpsData = gpsResult.data;
    HIHData hihData = hihResult.data;
    LM75Data lm75Data = lm75Result.data;
    MPUData mpuData = mpuResult.data;
    MS5Data ms5Data = ms5Result.data;
    if (this->mountFailed) {
        if (millis() - this->lastMountTry > 10000) {
            return this->configure();
        } else {
            return 5;
        }
    }
    /*DATE, TIME, LAT, LNG, ALT, SPEED, COURSE, SATELITES, HUMIDITY,
    //TEMPHUMIDITY, TEMPOUT, ACC_X, ACC_Y, ACC_Z, GYR_X; GYR_Y, GYR_Z, TEMPGYRO, PRESSURE, PRESSURETEMP*/
    char str[300];
    sprintf(str, "%06u, %09u,%f,%f,%f,%d,%f,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%d,%d,%u,%u,%u,%u,%u\n",
        gpsData.date.value(), gpsData.time.value()*10+gpsData.time.age(), gpsData.location.lat(), gpsData.location.lng(), gpsData.altitude.meters(), gpsData.speed.value(),
        gpsData.course.deg(), gpsData.satellites.value(), hihData.getHumidity(), hihData.getTemperature(),
        lm75Data.getTemperature(), mpuData.getAccX(), mpuData.getAccY(), mpuData.getAccZ(), mpuData.getGyroX(),
        mpuData.getGyroY(), mpuData.getGyroZ(), mpuData.getTemperature(), ms5Data.getPressure(), ms5Data.getTemperature(),
        gpsResult.status, hihResult.status, lm75Result.status, mpuResult.status, ms5Result.status);
    this->file.print(str);
    this->file.flush();
    return 1;
}

int32_t Storage::getFileCount() {
    File root = this->fs->open("/");
    if(!root){
        Serial.println("Failed to open directory");
        return -1;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return -1;
    }

    File file = root.openNextFile();
    int32_t index = 0;
    while(file){
        index++;
        file = root.openNextFile();
    }
    return index;
}