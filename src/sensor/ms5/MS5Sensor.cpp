#include "MS5Sensor.h"
#include <Wire.h>

MS5Data::MS5Data(int32_t pressure, int32_t temperature) {
    this->pressure = pressure;
    this->temperature = temperature;
}

MS5Data::MS5Data() {
    this->pressure = 0;
    this->temperature = 0;
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

DataResult<MS5Data> MS5Sensor::getData() {
  DataResult<int32_t> pressure = MS5Sensor::getMeasurement(PRESSURE);
  DataResult<int32_t> temperature = MS5Sensor::getMeasurement(TEMPERATURE);
  uint8_t status = (pressure.status == 1 && temperature.status == 1) ? 1 : 0;
  return {status, MS5Data(pressure.data, temperature.data)};
}

DataResult<int32_t> MS5Sensor::getMeasurement(measurement _measurement) {
  Wire.beginTransmission(MS5_ADDRESS);
  Wire.write(CMD_ADC_CONV+_measurement);
  uint8_t status = Wire.endTransmission(true) == 5 ? 0 : 1;
  delay(11);
  Wire.beginTransmission(MS5_ADDRESS);
  Wire.write(CMD_ADC_READ);
  Wire.endTransmission(true);
  Wire.requestFrom(MS5_ADDRESS, (size_t) 3, true);
  uint32_t measurement = (Wire.read() << 16) + ((uint32_t)Wire.read() << 8) + Wire.read();
  return {static_cast<uint8_t>(status == 0 ? 0 : 1), static_cast<int>(measurement)};
}

MS5CorrectedData MS5Sensor::correct(const MS5Data& data) {
  	int32_t temperature_raw = data.getTemperature();
	int32_t pressure_raw = data.getPressure();
	
  	//Create Variables for calculations
	int32_t temp_calc;
	int32_t pressure_calc;
	
	int32_t dT;
		
	//Now that we have a raw temperature, let's compute our actual.
	dT = temperature_raw - ((int32_t)coefficient[5] << 8);
	temp_calc = (((int32_t)dT * coefficient[6]) >> 23) + 2000;
	
	// TODO TESTING  _temperature_actual = temp_calc;
	
	//Now we have our first order Temperature, let's calculate the second order.
	int64_t T3, OFF2, SENS2, OFF, SENS; //working variables

	if (temp_calc < 2000) 
	// If temp_calc is below 20.0C
	{
        T3 = 3 * (((int64_t)dT * dT) >> 33);
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
        T3 = 7 * ((uint64_t)dT * dT) / pow(2, 37);
		OFF2 = ((temp_calc - 2000) * (temp_calc - 2000)) / 16;
		SENS2 = 0;
	}
	
	// Now bring it all together to apply offsets 
	
	OFF = ((int64_t)coefficient[2] << 16) + (((coefficient[4] * (int64_t)dT)) >> 7);
	SENS = ((int64_t)coefficient[1] << 15) + (((coefficient[3] * (int64_t)dT)) >> 8);
	
	temp_calc = temp_calc - T3;
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;

	// Now lets calculate the pressure
	

	pressure_calc = ((SENS * pressure_raw) / 2097152 - OFF) / 32768;

  	return {pressure_calc/10.0f, temp_calc/100.0f};
}

void MS5Data::print(MS5CorrectedData corrected) const {
	printf("\n===[ MS5 ]===\033[K\nTemperature: %d\033[K\nPressure: %d\033[K\nCorrected Temperature: %f\033[K\nCorrected Pressure: %f\033[K\n\033[K",
		this->temperature, this->pressure, corrected.temperature, corrected.pressure);
}

uint8_t MS5Data::convertLORA() {
    return 0;
}
