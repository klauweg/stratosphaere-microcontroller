#include "Display.h"

void Display::configure() {
    this->disp = new Adafruit_SSD1306(128, 64);
    this->disp->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    this->disp->setTextColor(WHITE);
    this->disp->setTextSize(1);
    this->render();
}

void Display::setBuffer(std::string buff) {
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