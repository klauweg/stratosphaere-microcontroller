#include "Display.h"

void Display::configure() {
    this->disp = new Adafruit_SSD1306(128, 64);
    this->disp->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    this->disp->setTextColor(WHITE);
    this->disp->setTextSize(1);
    this->render();
}

void Display::tick() {
    GPSData gpsData = this->gpsSensor->getData();
    LM75Data lm75Data = this->lm75Sensor->getData();
    char newbuffer[300];
    if (millis() % 20000 < 10000) {
        sprintf(newbuffer, "Lora: %u SD: %u\nSensor: %u\nUSat: %u ASat: %u\n%s",
                0, this->storage->getStatus(),
                this->gpsSensor->getStatus() << 4 | this->hihSensor->getStatus() << 3 | this->lm75Sensor->getStatus() << 2 |
                this->mpuSensor->getStatus() << 1 | this->ms5Sensor->getStatus(), gpsData.satellites.value(),
                gpsData.getVisibleSatellites(), this->storage->getFileName().c_str());
    } else {
        sprintf(newbuffer, "OTemp: %u\nDate: %u.%u.%u\nTime: %u.%u.%u\nLat: %2.4f\nLng: %2.4f",
                lm75Data.getTemperature(), gpsData.date.day(), gpsData.date.month(), gpsData.date.year(),
                gpsData.time.hour(), gpsData.time.minute(), gpsData.time.second(), gpsData.location.lat(),
                gpsData.location.lng());
    }
    gpsData.print();
    this->hihSensor->getData().print();
    lm75Data.print();
    this->mpuSensor->getData().print();
    this->ms5Sensor->getData().print();
    this->ms5Sensor->getData().print(this->ms5Sensor->getCorrectedData());
    this->setBuffer(newbuffer);
}

void Display::setBuffer(const std::string& buff) {
    if (buff == this->buffer) return;
    this->buffer = buff;
    this->render();
}

void Display::render() {
    this->disp->clearDisplay();
    this->disp->setCursor(0, 0);
    this->disp->print(this->buffer.c_str());
    this->disp->display();
}

Display::Display(GPSSensor *gps, LM75Sensor *lm75, HIHSensor *hih,
                 MPUSensor *mpu, MS5Sensor *ms5, Storage *sto, Lora::Lora *lora) {
    this->gpsSensor = gps;
    this->lm75Sensor = lm75;
    this->hihSensor = hih;
    this->mpuSensor = mpu;
    this->ms5Sensor = ms5;
    this->storage = sto;
    this->lora = lora;
}
