
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "GpsOnePPS.h"

int baro_enable=1;

char baro_type[32]="Generic";

int baro_rate=100;
int last_baro_time=0;

#define BME_ADDRESS	0x76

//#define SEALEVELPRESSURE_HPA (1013.25)
#define SEALEVELPRESSURE_HPA (1009.70)

Adafruit_BME280 bme;

int SetupBarometer(void)
{
	if(!bme.begin(BME_ADDRESS))
	{
		Serial.println("BME280 barometer not found, disabling");
		baro_enable=0;
		
		return(1);
	}	

	last_baro_time=millis_1pps();
	
	return(0);
}

void PollBarometer(void)
{
	if(baro_enable)
	{
		if(millis_1pps()>(last_baro_time+baro_rate))
		{
			Serial.print("Temperature = ");			Serial.print(bme.readTemperature());					Serial.print(" *C\t");
			Serial.print("Pressure = ");			Serial.print(bme.readPressure()/100.0F);				Serial.print(" hPa\t");
			Serial.print("Approx. Altitude = ");	Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));	Serial.print(" m\t");
			Serial.print("Humidity = ");			Serial.print(bme.readHumidity());						Serial.print(" %\t");
		
			Serial.println();
		
			if(logging_enable)
			{
			
			}
		
		
		}
	
	
	}
}

