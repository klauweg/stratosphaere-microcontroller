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

unsigned long lastMillis = 0;

Storage *storage = new Storage(&SD_MMC);
Display *display = new Display();
Lora::LoraSensor *lora = new Lora::LoraSensor();

GPSSensor *gpsSensor = new GPSSensor();
HIHSensor *hihSensor = new HIHSensor();
LM75Sensor *lm75Sensor = new LM75Sensor();
MPUSensor *mpuSensor = new MPUSensor();
MS5Sensor *ms5Sensor = new MS5Sensor();

uint8_t sdStatus = 0;


void printHex2(unsigned v) {
    v &= 0xff;
    if (v < 16)
        Serial.print('0');
    Serial.print(v, HEX);
}

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
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
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            {
                u4_t netid = 0;
                devaddr_t devaddr = 0;
                u1_t nwkKey[16];
                u1_t artKey[16];
                LMIC_getSessionKeys(&netid, &devaddr, nwkKey, artKey);
                Serial.print("netid: ");
                Serial.println(netid, DEC);
                Serial.print("devaddr: ");
                Serial.println(devaddr, HEX);
                Serial.print("AppSKey: ");
                for (size_t i=0; i<sizeof(artKey); ++i) {
                    if (i != 0)
                        Serial.print("-");
                    printHex2(artKey[i]);
                }
                Serial.println("");
                Serial.print("NwkSKey: ");
                for (size_t i=0; i<sizeof(nwkKey); ++i) {
                    if (i != 0)
                        Serial.print("-");
                    printHex2(nwkKey[i]);
                }
                Serial.println();
            }
            LMIC_setLinkCheckMode(0);
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
                Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
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
            break;

        default:
            Serial.print(F("Unknown event: "));
            Serial.println((unsigned) ev);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    Wire.begin();
    Wire.setClock(100000);

    lora->configure();
    display->configure();
    sdStatus = storage->configure();

    gpsSensor->configure();
    hihSensor->configure();
    lm75Sensor->configure();
    mpuSensor->configure();
    ms5Sensor->configure();
    //Serial.print("\033[2J");    // clear screen command
}

void loop() {
    if (lora->canWork(250)) {
        lastMillis = millis();

        DataResult<GPSData> gpsResult = gpsSensor->measure();
        DataResult<HIHData> hihResult = hihSensor->measure();
        DataResult<LM75Data> lm75Result = lm75Sensor->measure();
        DataResult<MPUData> mpuResult = mpuSensor->measure();
        DataResult<MS5Data> ms5Result = ms5Sensor->measure();

        GPSData gpsData = gpsResult.data;
        LM75Data lm75Data = lm75Result.data;
        HIHData hihData = hihResult.data;
        MS5Data ms5Data = ms5Result.data;
        MPUData mpuData = mpuResult.data;
        MS5CorrectedData ms5CorrectedData = ms5Sensor->correct(ms5Data);

        sdStatus = storage->storeData(gpsResult, hihResult, lm75Result, mpuResult, ms5Result);

        if (millis() - lora->getLastSent() >= 60000) {
            lora->sendData(gpsData, hihData, lm75Data, ms5CorrectedData);
        }

        char buffer[300];
        if (millis() % 20000 < 10000) {
            sprintf(buffer, "LoraSensor: %u SD: %u\nSensor: %u\nUSat: %u ASat: %u\n%s",
                    0, sdStatus,
                    gpsResult.status << 4 | hihResult.status << 3 | lm75Result.status << 2 | mpuResult.status << 1 |
                    ms5Result.status, gpsData.satellites.value(),
                    gpsData.getVisibleSatellites(), storage->getFileName().c_str());
        } else {
            sprintf(buffer, "OTemp: %u\nDate: %u.%u.%u\nTime: %u.%u.%u\nLat: %2.4f\nLng: %2.4f",
                    lm75Data.getTemperature(), gpsData.date.day(), gpsData.date.month(), gpsData.date.year(),
                    gpsData.time.hour(), gpsData.time.minute(), gpsData.time.second(), gpsData.location.lat(),
                    gpsData.location.lng());
        }
        display->setBuffer(buffer);

        /*Serial.print("\033[f");
        gpsData.print();
        hihData.print();
        lm75Data.print();
        mpuData.print();
        ms5Data.print(ms5Sensor->correct(ms5Data));
        printf("\n===[ DEBUG ]===\033[K\033[K\nLast Tick Duration: %lu\033[K\nCalculated Time in Millis: %u\033[K\n\033[K",
               millis() - lastMillis, gpsData.time.value() * 10 + gpsData.time.age());*/
    }
    os_runloop_once();
}