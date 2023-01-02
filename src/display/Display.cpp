#include "Display.h"

void Display::configure() {
    this->disp = new Adafruit_SSD1306(128, 64);
    this->disp->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    this->disp->setTextColor(WHITE);
    this->disp->setTextSize(1);
    this->render();
}

void Display::setLorawanStatus(uint8_t status) {
    if (status == this->lorawanStatus) return;
    this->lorawanStatus = status;
    this->render();
}

void Display::setUsedSatelliteStatus(uint32_t status) {
    if (status == this->usedsatellites) return;
    this->usedsatellites = status;
    this->render();
}

void Display::setAvSatelliteStatus(uint8_t status) {
    if (status == this->avsatellites) return;
    this->avsatellites = status;
    this->render();
}

void Display::setSensorStatus(uint16_t status) {
    if (status == this->sensorStatus) return;
    this->sensorStatus = status;
    this->render();
}

void Display::setSdStatus(uint8_t status) {
    if (status == this->sdStatus) return;
    this->sdStatus = status;
    this->render();
}

void Display::setFileName(std::string name) {
    if (name == this->fileName) return;
    this->fileName = name;
    this->render();
}

void Display::render() {
    this->disp->clearDisplay();
    this->disp->setCursor(0, 0);
    this->disp->printf("Lora: %u SD: %u\nSensor: %u\nUSat: %u ASat: %u\n%s\nMillis: %lu",
        this->lorawanStatus, this->sdStatus, this->sensorStatus, this->usedsatellites, 
        this->avsatellites, this->fileName.c_str(), millis());
    this->disp->display();
}