#include <Adafruit_SSD1306.h>
#include <string>
#include "sensor/ms5/MS5Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include "storage/storage.h"
#include "lora/lora.h"

#ifndef DISPLAY_H
#define DISPLAY_H

class Display : public Module {
    protected:
        GPSSensor *gpsSensor;
        LM75Sensor *lm75Sensor;
        HIHSensor *hihSensor;
        MPUSensor *mpuSensor;
        MS5Sensor *ms5Sensor;
        Storage *storage;
        Lora::Lora *lora;
        Adafruit_SSD1306* disp;
        std::string buffer;
        void render();
    public:
        Display(GPSSensor*, LM75Sensor*, HIHSensor*, MPUSensor*, MS5Sensor*, Storage*, Lora::Lora*);
        void setBuffer(const std::string& buffer);
        void configure() override;
        void tick() override;
};

#endif