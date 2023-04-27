#include "storage.h"

Storage::Storage(fs::SDMMCFS *fileSystem, GPSSensor *gps, LM75Sensor *lm75, HIHSensor *hih, MPUSensor *mpu, MS5Sensor *ms5) {
    this->fs = fileSystem;
    this->gpsSensor = gps;
    this->lm75Sensor = lm75;
    this->hihSensor = hih;
    this->mpuSensor = mpu;
    this->ms5Sensor = ms5;
}

void Storage::configure() {
    this->lastMountTry = millis();
    this->fs->end();
    if(!this->fs->begin("/sdcard", true)){
        this->status = 10;
    }
    int32_t fileCount = Storage::getFileCount();
    if (fileCount != -1) {
        this->fileName = "/stratodata-" + std::to_string(fileCount) + ".txt";
        this->file = this->fs->open(this->fileName.c_str(), FILE_APPEND);
        if(file){
            this->mountFailed = false;
            this->status = 1;
            return;
        }
    }
    this->mountFailed = true;
    this->status = 11;
}

void Storage::tick() {
    GPSData gpsData = gpsSensor->getData();
    HIHData hihData = hihSensor->getData();
    LM75Data lm75Data = lm75Sensor->getData();
    MPUData mpuData = mpuSensor->getData();
    MS5Data ms5Data = ms5Sensor->getData();
    if (this->mountFailed) {
        if (millis() - this->lastMountTry > 10000) {
            return this->configure();
        } else {
            this->status = 12;
        }
    }
    char str[300];
    sprintf(str, "%lu,%06u,%09u,%f,%f,%f,%d,%f,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u,%u,%u,%u,%u\n",
            millis(), gpsData.date.value(), gpsData.time.value(), gpsData.location.lat(), gpsData.location.lng(), gpsData.altitude.meters(), gpsData.speed.value(),
            gpsData.course.deg(), gpsData.satellites.value(), hihData.getHumidity(), hihData.getTemperature(),
            lm75Data.getTemperature(), mpuData.getAccX(), mpuData.getAccY(), mpuData.getAccZ(), mpuData.getGyroX(),
            mpuData.getGyroY(), mpuData.getGyroZ(), mpuData.getTemperature(), ms5Data.getPressure(), ms5Data.getTemperature(),
            gpsSensor->getStatus(), hihSensor->getStatus(), lm75Sensor->getStatus(), mpuSensor->getStatus(), ms5Sensor->getStatus());
    this->file.print(str);
    this->file.flush();
    this->status = 1;
}

int32_t Storage::getFileCount() {
    File root = this->fs->open("/");
    if(!root){
        Serial.println("Failed to open directory");
        this->status = 13;
        return -1;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        this->status = 14;
        return -1;
    }

    File openFile = root.openNextFile();
    int32_t index = 0;
    while(openFile){
        index++;
        openFile = root.openNextFile();
    }
    return index;
}