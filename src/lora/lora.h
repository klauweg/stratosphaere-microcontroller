#include "sensor/ms5/MS5Sensor.h"
#include "sensor/mpu/MPUSensor.h"
#include "sensor/lm75/LM75Sensor.h"
#include "sensor/hih/HIHSensor.h"
#include "sensor/gps/GPSSensor.h"
#include <arduino_lmic.h>
#include "hal/hal.h"

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
            .dio = {26},
    };

    class LoraSensor {
        private:
            long lastSent = 0;
            void sendMessage(uint8_t *);
            void setBits(uint8_t*, uint8_t, uint8_t, uint16_t);
        public:
            long getLastSent() {return this->lastSent;};
            void configure();
            bool canWork(uint16_t);
            void sendData(GPSData&, HIHData&, LM75Data&, MS5CorrectedData&);
    };
}