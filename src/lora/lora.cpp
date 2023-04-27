#include "lora.h"

namespace Lora {
    void Lora::configure() {
        // LMIC init
        os_init_ex(&lmic_pins);
        // Reset the MAC state. Session and pending data transfers will be discarded.
        LMIC_reset();
        LMIC_startJoining();
    }

    void Lora::tick() {
        if (millis() - this->lastSent <= 60000) return;
        double altitude = gpsSensor->getData().altitude.meters();
        double latitude = gpsSensor->getData().location.lat();
        double longitude = gpsSensor->getData().location.lng();
        float pressure = ms5Sensor->getCorrectedData().getPressure();
        float humidity = hihSensor->getCorrectedData().getHumidity();
        int16_t temperature = lm75Sensor->getData().getTemperature();
        uint16_t raw_altitude = round((altitude + 100)/altitude_factor);
        uint16_t raw_latitude = round((latitude - center_latitude + max_pos_delta) * 1000);
        uint16_t raw_longitude = round((longitude - center_longitude + max_pos_delta) * 1000);
        uint8_t raw_pressure = round((pressure/10)/pressure_factor);
        uint8_t raw_temperature = round(temperature/2.0f+temperature_offset);
        uint8_t raw_humidity = round(humidity/humidity_factor);
        uint8_t bytes[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        setBits(bytes, 0, 12, raw_altitude);
        setBits(bytes, 12, 15, raw_latitude);
        setBits(bytes, 27, 15, raw_longitude);
        setBits(bytes, 42, 8, raw_pressure);
        setBits(bytes, 50, 7, raw_temperature);
        setBits(bytes, 57, 7, raw_humidity);
        this->sendMessage(bytes);
        this->lastSent = millis();
    }

    Lora::Lora(GPSSensor *gps, LM75Sensor *lm75, HIHSensor *hih, MS5Sensor *ms5) {
        this->gpsSensor = gps;
        this->lm75Sensor = lm75;
        this->hihSensor = hih;
        this->ms5Sensor = ms5;
    }

    void Lora::sendMessage(uint8_t *bytes) {
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

    void Lora::setBits(uint8_t *array, uint8_t start_bit, uint8_t value_length, uint16_t value) {
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

    bool Lora::canWork(uint16_t millis) {
        return !os_queryTimeCriticalJobs(ms2osticks(millis));
    }
}