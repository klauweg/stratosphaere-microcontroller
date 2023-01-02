#include <Adafruit_SSD1306.h>
#include <string>

class Display {
    protected:
        Adafruit_SSD1306* disp;
        std::string buffer;
        void render();
    public:
        void setBuffer(std::string buffer);
        void configure();
};