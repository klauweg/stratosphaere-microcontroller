//#include "utilities.h"
#include <Arduino.h>

void setup()
{
  //initBoard();
  Serial.begin(115200);
  Serial1.begin(9600, SERIAL_8N1, 34, 12);
}

void loop()
{
  while (Serial.available())
  {
    Serial1.write(Serial.read());
  }
  while (Serial1.available())
  {
    Serial.write(Serial1.read());
  }
}