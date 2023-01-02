#include <Adafruit_SSD1306.h>
#include <string>

class Display {
    protected:
        Adafruit_SSD1306* disp;
        uint8_t lorawanStatus = 0;
        uint16_t sensorStatus = 0;
        uint32_t usedsatellites = 0;
        uint8_t avsatellites = 0;
        uint8_t sdStatus = 0;
        std::string fileName = "unknown.png";
        void render();
    public:
        void configure();
        void setLorawanStatus(uint8_t status);
        void setUsedSatelliteStatus(uint32_t status);
        void setAvSatelliteStatus(uint8_t status);
        void setSensorStatus(uint16_t status);
        void setSdStatus(uint8_t status);
        void setFileName(std::string name);
};