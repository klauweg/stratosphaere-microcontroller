#include "storage.h"


uint8_t Storage::configure() {
    this->lastMountTry = millis();
    SD_MMC.end();
    if(!SD_MMC.begin("/sdcard", true)){
        return 2;
    }
    int32_t fileCount = this->getFileCount();
    if (fileCount == -1) {
        return 3;
    }
    this->fileName = "/stratodata-" + std::to_string(fileCount) + ".txt";
    this->mountFailed = false;
    return 1;
}

uint8_t Storage::storeData(DataResult<GPSData> gpsResult, 
        DataResult<HIHData> hihResult, 
        DataResult<LM75Data> lm75Result, 
        DataResult<MPUData> mpuResult,
        DataResult<MS5Data> ms5Result) {
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
    File file = SD_MMC.open(fileName.c_str(), FILE_APPEND);
    if(!file){
        this->mountFailed = true;
        return 4;
    }
    /*DATE, TIME, LAT, LNG, ALT, SPEED, COURSE, SATELITES, HUMIDITY, 
    //TEMPHUMIDITY, TEMPOUT, ACC_X, ACC_Y, ACC_Z, GYR_X; GYR_Y, GYR_Z, TEMPGYRO, PRESSURE, PRESSURETEMP*/
    char str[300];
    sprintf(str, "%06u, %09u,%f,%f,%f,%f,%f,%u,%d,%d,%d,%d,%d,%d,%d,%d,%d,%f,%d,%d,%u,%u,%u,%u,%u\n",
        gpsData.date.value(), gpsData.time.value()*10+gpsData.time.age(), gpsData.location.lat(), gpsData.location.lng(), gpsData.altitude.meters(), gpsData.speed.value(),
        gpsData.course.deg(), gpsData.satellites.value(), hihData.getHumidity(), hihData.getTemperature(),
        lm75Data.getTemperature(), mpuData.getAccX(), mpuData.getAccY(), mpuData.getAccZ(), mpuData.getGyroX(),
        mpuData.getGyroY(), mpuData.getGyroZ(), mpuData.getTemperature(), ms5Data.getPressure(), ms5Data.getTemperature(),
        gpsResult.status, hihResult.status, lm75Result.status, mpuResult.status, ms5Result.status);
    file.print(str);
    file.close();
    return 1;
}

int32_t Storage::getFileCount() {
    File root = SD_MMC.open("/");
    if(!root){
        Serial.println("Failed to open directory");
        return -1;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return -1;
    }

    File file = root.openNextFile();
    uint32_t index = 0;
    while(file){
        index++;
        file = root.openNextFile();
    }
    return index;
}