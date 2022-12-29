//#include "utilities.h"
#include <serial.h>

void setup()
{
   //initBoard();
   Serial.begin(115200);
   Serial1.begin(9600);
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