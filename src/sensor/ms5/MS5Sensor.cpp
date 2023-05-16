#include "MS5Sensor.h"
#include <Wire.h>

MS5Data::MS5Data(uint32_t pressure, uint32_t temperature) {
    this->pressure = pressure;
    this->temperature = temperature;
}
MS5Data::MS5Data() {
    this->pressure = 0;
    this->temperature = 0;
}
MS5CorrectedData::MS5CorrectedData(float pressure, float temperature) {
    this->pressure = pressure;
    this->temperature = temperature;
}
MS5CorrectedData::MS5CorrectedData() {
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

void MS5Sensor::measure() {
  uint32_t pressure = MS5Sensor::getMeasurement(PRESSURE);
  uint8_t pstatus = this->getStatus();
  uint32_t temperature = MS5Sensor::getMeasurement(TEMPERATURE);
  uint8_t tstatus = this->getStatus();
  this->status = (pstatus == 1 && tstatus == 1) ? 1 : 0;
  this->data = MS5Data{pressure, temperature};
  this->correct();
}

int32_t MS5Sensor::getMeasurement(measurement _measurement) {
  Wire.beginTransmission(MS5_ADDRESS);
  Wire.write(CMD_ADC_CONV+_measurement);
  this->status = Wire.endTransmission(true) == 0 ? 1 : 0;
  delay(11);
  Wire.beginTransmission(MS5_ADDRESS);
  Wire.write(CMD_ADC_READ);
  Wire.endTransmission(true);
  Wire.requestFrom(MS5_ADDRESS, (size_t) 3, true);
  uint32_t measurement = ((uint32_t)Wire.read() << 16) + ((uint32_t)Wire.read() << 8) + Wire.read();
  return measurement;
}

void MS5Sensor::correct() {

    // Create array to hold the 8 sensor calibration coefficients
    //static unsigned int      sensorCoeffs[8]; // unsigned 16-bit integer (0-65535)
    // D1 and D2 need to be unsigned 32-bit integers (long 0-4294967295)
    uint32_t     D1 = this->data.getPressure();    // Store uncompensated pressure value
    uint32_t     D2 = this->data.getTemperature();    // Store uncompensated temperature value
    // These three variables are used for the conversion steps
    // They should be signed 32-bit integer initially
    // i.e. signed long from -2147483648 to 2147483647
    int32_t	dT;
    int32_t 	TEMP;
    // These values need to be signed 64-bit integers
    // (long = int64_t)
    int64_t	Offset;
    int64_t	Sensitivity;
    int64_t	T22;
    int64_t	OFF2;
    int64_t	Sens2;

    // Calculate 1st order temperature, dT is a long signed integer
    // D2 is originally cast as an uint32_t, but can fit in a int32_t, so we'll
    // cast both parts of the equation below as signed values so that we can
    // get a negative answer if needed
    dT = (int32_t)D2 - ( (int32_t)this->coefficient[5] * 256 );
    // Use integer division to calculate TEMP. It is necessary to cast
    // one of the operands as a signed 64-bit integer (int64_t) so there's no
    // rollover issues in the numerator.
    TEMP = 2000 + ((int64_t)dT * this->coefficient[6]) / 8388608LL;
    // Recast TEMP as a signed 32-bit integer
    TEMP = (int32_t)TEMP;


    // All operations from here down are done as integer math until we make
    // the final calculation of pressure in mbar.


    // Do 2nd order temperature compensation (see pg 9 of MS5803 data sheet)
    // I have tried to insert the fixed values wherever possible
    // (i.e. 2^31 is hard coded as 2147483648).
    if (TEMP < 2000) {
        // For 1 bar model
        T22 = ((int64_t)dT * dT) / 2147483648ULL ; // 2^31 = 2147483648
        T22 = (int32_t)T22; // recast as signed 32bit integer
        OFF2 = 3 * ((TEMP-2000) * (TEMP-2000));
        Sens2 = 7 * ((TEMP-2000)*(TEMP-2000)) / 8 ;
    } else { // if TEMP is > 2000 (20.0C)
        // For 1 bar model
        T22 = 0;
        OFF2 = 0;
        Sens2 = 0;
        if (TEMP > 4500) {
            // Extra adjustment for high temps, only needed for 1 bar model
            Sens2 = Sens2 - ((TEMP-4500)*(TEMP-4500)) / 8;
        }
    }

    // Additional compensation for very low temperatures (< -15C)
    if (TEMP < -1500) {
        // For 1 bar model
        // Leave OFF2 alone in this case
        Sens2 = Sens2 + 2 * ((TEMP+1500)*(TEMP+1500));
    }

    // Calculate initial Offset and Sensitivity
    // Notice lots of casts to int64_t to ensure that the
    // multiplication operations don't overflow the original 16 bit and 32-bit
    // integers

    // For 1 bar sensor
    Offset = (int64_t)this->coefficient[2] * 65536 + (this->coefficient[4] * (int64_t)dT) / 128;
    Sensitivity = (int64_t)this->coefficient[1] * 32768 + (this->coefficient[3] * (int64_t)dT) / 256;

    // Adjust TEMP, Offset, Sensitivity values based on the 2nd order
    // temperature correction above.
    TEMP = TEMP - T22; // both should be int32_t
    Offset = Offset - OFF2; // both should be int64_t
    Sensitivity = Sensitivity - Sens2; // both should be int64_t

    // Final compensated pressure calculation. We first calculate the pressure
    // as a signed 32-bit integer (mbarInt), then convert that value to a
    // float (mbar).

    // For 1 bar sensor
    int32_t mbarInt = ((D1 * Sensitivity) / 2097152 - Offset) / 32768;
    float mbar = (float)mbarInt / 100;

    // Calculate the human-readable temperature in Celsius
    float tempC  = (float)TEMP / 100;

  	this->correctedData = MS5CorrectedData{mbar, tempC};
}

void MS5Data::print(MS5CorrectedData corrected) const {
	printf("\n===[ MS5 ]===\033[K\nTemperature: %d\033[K\nPressure: %d\033[K\nCorrected Temperature: %f\033[K\nCorrected Pressure: %f\033[K\n\033[K",
		this->temperature, this->pressure, corrected.getTemperature(), corrected.getPressure());
}