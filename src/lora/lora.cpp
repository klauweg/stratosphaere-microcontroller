#include "lora.h"

namespace Lora {
    void LoraSensor::configure() {
        // LMIC init
        os_init_ex(&lmic_pins);
        // Reset the MAC state. Session and pending data transfers will be discarded.
        LMIC_reset();
        LMIC_startJoining();
    }

    void LoraSensor::sendData(GPSData& gpsData, HIHData& hihData, LM75Data& lm75Data, MS5CorrectedData& ms5Data) {
        double altitude = gpsData.altitude.meters();
        double latitude = gpsData.location.lat();
        double longitude = gpsData.location.lng();
        double pressure = ms5Data.getPressure();
        int16_t humidity = hihData.getHumidity();
        int16_t temperature = lm75Data.getTemperature();
        uint16_t raw_altitude = round((altitude + 100)/altitude_factor);
        uint16_t raw_latitude = round((latitude - center_latitude + max_pos_delta) * 1000);
        uint16_t raw_longitude = round((longitude - center_longitude + max_pos_delta) * 1000);
        uint8_t raw_pressure = round((pressure/10)/pressure_factor);
        uint8_t raw_temperature = round(temperature+temperature_offset);
        uint8_t raw_humidity = round(humidity/humidity_factor);
        uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        this->setBits(bytes, 0, 12, raw_altitude);
        this->setBits(bytes, 12, 15, raw_latitude);
        this->setBits(bytes, 27, 15, raw_longitude);
        this->setBits(bytes, 42, 8, raw_pressure);
        this->setBits(bytes, 50, 7, raw_temperature);
        this->setBits(bytes, 57, 7, raw_humidity);
        this->sendMessage(bytes);
        this->lastSent = millis();
    }

    void LoraSensor::sendMessage(uint8_t *bytes) {
        // Check if there is not a current TX/RX job running
        if (LMIC.opmode & OP_TXRXPEND) {
            Serial.println(F("OP_TXRXPEND, not sending"));
        } else {
            // Prepare upstream data transmission at the next possible time.
            LMIC_setTxData2(1, bytes, 7, 0);
            Serial.println(F("Packet queued"));
        }
        // Next TX is scheduled after TX_COMPLETE event.
    }

    void LoraSensor::setBits(uint8_t *array, uint8_t start_bit, uint8_t value_length, uint16_t value) {
        for (int i = start_bit; i < start_bit+value_length; i++) {
            const int byteIndex = floor(i/8);
            if (byteIndex >= 8) return;
            const uint8_t byteBitIndex = (i%8) + 1;
            if (value & (1 << (value_length - (i-start_bit) - 1))) {
                array[byteIndex] = array[byteIndex] | (1 << (8 - byteBitIndex));
            } else {
                array[byteIndex] = array[byteIndex] & ~(1 << (8 - byteBitIndex));
            }
        }
    }

    bool LoraSensor::canWork(uint16_t millis) {
        return !os_queryTimeCriticalJobs(ms2osticks(millis));
    }
}