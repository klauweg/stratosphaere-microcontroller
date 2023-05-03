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
    HIHCorrectedData hihData = this->hihSensor->getCorrectedData();
    char newbuffer[300];
    if (millis() % 20000 < 10000) {
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
    } else {
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
    }
    this->setBuffer(newbuffer);
    /*gpsData.print();
    this->hihSensor->getData().print();
    lm75Data.print();
    this->mpuSensor->getData().print();
    this->ms5Sensor->getData().print();
    this->ms5Sensor->getData().print(this->ms5Sensor->getCorrectedData());*/
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
