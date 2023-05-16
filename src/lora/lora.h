#include "sensor/ms5/MS5Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include <arduino_lmic.h>
#include "hal/hal.h"

#ifndef LORA_H
#define LORA_H

namespace Lora {
    const double center_latitude = 49.317;
    const double center_longitude = 11.023;
    const double max_pos_delta = 16.384;
    const double altitude_factor = 12.2314453;
    const double pressure_factor = 5.859375;
    const double humidity_factor = 0.793650794;
    const double temperature_offset = 90;

    // Pin mapping
    const lmic_pinmap lmic_pins = {
            .nss = 18,
            .rxtx = LMIC_UNUSED_PIN,
            .rst = 23,
            .dio = {26, 33, 32},
    };

    class Lora : public Module {
        private:
            GPSSensor *gpsSensor;
            LM75Sensor *lm75Sensor;
            HIHSensor *hihSensor;
            MS5Sensor *ms5Sensor;
            long lastSent = 0;
            void sendMessage(uint8_t *);
            static void setBits(uint8_t*, uint8_t, uint8_t, uint16_t);
        public:
            Lora(GPSSensor*, LM75Sensor*, HIHSensor*, MS5Sensor*);
            void configure() override;
            void setStatus(uint8_t status) {this->status = status;};
            void tick() override;
            bool canWork(uint16_t);
    };
}

#endif