#include <stdint.h>

template <class D>
class Sensor {
  public:
    void configure();
    D getData();
};

class SensorData {
  public:
    uint8_t convertLORA();
};