#include "Display.h"

void Display::configure() {
    this->disp = new Adafruit_SSD1306(128, 64);
    this->disp->begin(SSD1306_SWITCHCAPVCC, 0x3C);
    this->disp->setTextColor(WHITE);
    this->disp->setTextSize(1);
}

void Display::tick() {
    GPSData gpsData = this->gpsSensor->getData();
    HIHCorrectedData hihData = this->hihSensor->getCorrectedData();
    if (millis() - this->last_state_change > 5000) {
        this->last_state_change = millis();
        this->display_state = (this->display_state + 1) % 2;

        char newbuffer[300];

        if (this->display_state == 0) {
            sprintf(newbuffer, "GPS: %2.2f %2.2f %u\n"
                               "LM: %.1f\n"
                               "HIH: %.1f %.0f\n"
                               "MPU: %d %d\n"
                               "MS5: %.3f %.1f",
                    gpsData.location.lat(), gpsData.location.lng(), gpsData.time.second(),
                    this->lm75Sensor->getCorrectedData().getTemperature(),
                    hihData.getTemperature(), hihData.getHumidity(),
                    this->mpuSensor->getData().getGyroX(), this->mpuSensor->getData().getAccX(),
                    this->ms5Sensor->getCorrectedData().getPressure(), this->ms5Sensor->getCorrectedData().getTemperature()
            );
        } else if (this->display_state == 1) {
            sprintf(newbuffer, "Lora: %u\n"
                               "SD: %u\n"
                               "F: %s\n",
                    this->lora->getStatus(), this->storage->getStatus(), this->storage->getFileName().c_str()
            );
        }
        this->render(newbuffer);
    }
}

void Display::render(const char* buff) {
    this->disp->clearDisplay();
    this->disp->setCursor(0, 0);
    this->disp->print(buff);
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
