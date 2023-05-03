#include <Arduino.h>
#include <Wire.h>
#include "storage/storage.h"
#include "display/Display.h"
#include "lora/lora.h"

//lsb
static const u1_t PROGMEM APPEUI[8]={ 0xBB, 0xCD, 0x1A, 0xF9, 0xEB, 0x1A, 0x92, 0xAB };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}
//lsb
static const u1_t PROGMEM DEVEUI[8]={ 0x22, 0xC0, 0xA0, 0x92, 0xC9, 0xCC, 0xAB, 0x73 };
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}
//msb
static const u1_t PROGMEM APPKEY[16] = { 0x76, 0xF6, 0xF9, 0x4B, 0x88, 0xB0, 0xA6, 0x84, 0xAF, 0xBA, 0x13, 0x51, 0x63, 0xF3, 0x43, 0xEC };
void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

GPSSensor *gpsSensor = new GPSSensor();
HIHSensor *hihSensor = new HIHSensor();
LM75Sensor *lm75Sensor = new LM75Sensor();
MPUSensor *mpuSensor = new MPUSensor();
MS5Sensor *ms5Sensor = new MS5Sensor();
Lora::Lora *lora = new Lora::Lora(gpsSensor, lm75Sensor, hihSensor, ms5Sensor);
Storage *storage = new Storage(&SD_MMC, gpsSensor, lm75Sensor, hihSensor, mpuSensor, ms5Sensor);
Display *display = new Display(gpsSensor, lm75Sensor, hihSensor, mpuSensor, ms5Sensor, storage, lora);


void onEvent(ev_t ev)
{
    Serial.print(os_getTime());
    Serial.print(": ");
    switch (ev)
    {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            lora->setStatus(1);
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            lora->setStatus(5);
            // Disable link check validation (automatically enabled
            // during join, but because slow data rates change max TX
            // size, we don't use it in this example.
            LMIC_setLinkCheckMode(0);
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            lora->setStatus(2);
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
                Serial.println(F("Received ack"));
            if (LMIC.dataLen)
            {
                Serial.print(F("Received "));
                Serial.print(LMIC.dataLen);
                Serial.println(F(" bytes of payload"));
            }
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
            /*
            || This event is defined but not used in the code. No
            || point in wasting codespace on it.
            ||
            || case EV_SCAN_FOUND:
            ||    Serial.println(F("EV_SCAN_FOUND"));
            ||    break;
            */
        case EV_TXSTART:
            Serial.println(F("EV_TXSTART"));
            break;
        case EV_TXCANCELED:
            Serial.println(F("EV_TXCANCELED"));
            break;
        case EV_RXSTART:
            /* do not print anything -- it wrecks timing */
            break;
        case EV_JOIN_TXCOMPLETE:
            Serial.println(F("EV_JOIN_TXCOMPLETE: no JoinAccept"));
            lora->setStatus(3);
            break;

        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned)ev);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(100000);

    gpsSensor->configure();
    hihSensor->configure();
    lm75Sensor->configure();
    mpuSensor->configure();
    ms5Sensor->configure();
    lora->configure();
    storage->configure();
    display->configure();

    Serial.print("\033[2J");    // clear screen command
}
unsigned long lastMillis = 0;
void loop() {
    if (lora->canWork(250)) {
        lastMillis = millis();

        gpsSensor->tick();
        lm75Sensor->tick();
        hihSensor->tick();
        mpuSensor->tick();
        ms5Sensor->tick();
        storage->tick();
        lora->tick();
        display->tick();

        Serial.print("\033[f");
        printf("\n===[ DEBUG ]===\033[K\033[K\nTick Duration: %lu\033[K\n\033[K\n", millis() - lastMillis);
    }
    os_runloop_once();
}