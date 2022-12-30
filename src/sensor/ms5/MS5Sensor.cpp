#include "MS5Sensor.h"
#include <Wire.h>
#include <Arduino.h>

MS5Data::MS5Data(int32_t pressure, int32_t temperature) {
    this->pressure = pressure;
    this->temperature = temperature;
}

void MS5Sensor::configure() {
	uint8_t i;
	for(i = 0; i <= 7; i++) {
    Wire.beginTransmission(MS5_ADDRESS);
    Wire.write(CMD_PROM+(i*2));
    Wire.endTransmission(true);
    Wire.requestFrom( MS5_ADDRESS, (size_t) 2, true);
    uint8_t highByte = Wire.read(); 
    uint8_t lowByte = Wire.read();
    this->coefficient[i] = (highByte << 8)|lowByte;
	}
}

MS5Data MS5Sensor::getData() {
  int32_t pressure = this->getMeasurement(PRESSURE);
  int32_t temperature = this->getMeasurement(TEMPERATURE);
  return {pressure, temperature};
}

int32_t MS5Sensor::getMeasurement(measurement _measurement) {
  Wire.beginTransmission(MS5_ADDRESS);
  Wire.write(CMD_ADC_CONV+_measurement);
  Wire.endTransmission(true);
  delay(11);
  Wire.beginTransmission(MS5_ADDRESS);
  Wire.write(CMD_ADC_READ);
  Wire.endTransmission(true);
  Wire.requestFrom(MS5_ADDRESS, (size_t) 3, true);
  int32_t measurement = ((uint32_t)Wire.read() << 16) + ((uint32_t)Wire.read() << 8) + Wire.read();
  return measurement;
}

MS5CorrectedData MS5Sensor::correct(MS5Data data) {
  int32_t temperature_raw = data.temperature;
	int32_t pressure_raw = data.pressure;
	
  //Create Variables for calculations
	int32_t temp_calc;
	int32_t pressure_calc;
	
	int32_t dT;
		
	//Now that we have a raw temperature, let's compute our actual.
	dT = temperature_raw - ((int32_t)coefficient[5] << 8);
	temp_calc = (((int64_t)dT * coefficient[6]) >> 23) + 2000;
	
	// TODO TESTING  _temperature_actual = temp_calc;
	
	//Now we have our first order Temperature, let's calculate the second order.
	int64_t T2, OFF2, SENS2, OFF, SENS; //working variables

	if (temp_calc < 2000) 
	// If temp_calc is below 20.0C
	{	
		T2 = 3 * (((int64_t)dT * dT) >> 33);
		OFF2 = 3 * ((temp_calc - 2000) * (temp_calc - 2000)) / 2;
		SENS2 = 5 * ((temp_calc - 2000) * (temp_calc - 2000)) / 8;
		
		if(temp_calc < -1500)
		// If temp_calc is below -15.0C 
		{
			OFF2 = OFF2 + 7 * ((temp_calc + 1500) * (temp_calc + 1500));
			SENS2 = SENS2 + 4 * ((temp_calc + 1500) * (temp_calc + 1500));
		}
    } 
	else
	// If temp_calc is above 20.0C
	{ 
		T2 = 7 * ((uint64_t)dT * dT)/pow(2,37);
		OFF2 = ((temp_calc - 2000) * (temp_calc - 2000)) / 16;
		SENS2 = 0;
	}
	
	// Now bring it all together to apply offsets 
	
	OFF = ((int64_t)coefficient[2] << 16) + (((coefficient[4] * (int64_t)dT)) >> 7);
	SENS = ((int64_t)coefficient[1] << 15) + (((coefficient[3] * (int64_t)dT)) >> 8);
	
	temp_calc = temp_calc - T2;
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;

	// Now lets calculate the pressure
	

	pressure_calc = (((SENS * pressure_raw) / 2097152 ) - OFF) / 32768;

  return {pressure_calc/10.0f, temp_calc/100.0f};
}

void MS5Data::print(MS5CorrectedData corrected) {
	Serial.println("===[ MS5 ]===");
	Serial.print("Temperature: ");
	Serial.println(this->temperature);
	Serial.print("Pressure: ");
	Serial.println(this->pressure);
	Serial.print("Corrected Temperature: ");
	Serial.println(corrected.temperature);
	Serial.print("Corrected Pressure: ");
	Serial.println(corrected.pressure);
	Serial.print("\n");
}