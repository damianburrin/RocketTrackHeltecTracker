
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

int baro_enable=1;

char baro_type[32]="Generic";

int baro_rate=100;

#define BME_ADDRESS	0x76

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

int SetupBarometer(void)
{
	if(!bme.begin(BME_ADDRESS))
	{
		Serial.println("BME280 barometer not found, disabling");
		baro_enable=0;
		
		return(1);
	}	
	
	return(0);
}

void PollBarometer(void)
{
	if(baro_enable)
	{
		Serial.print("Temperature = ");
		Serial.print(bme.readTemperature());
		Serial.println(" *C");
		
		Serial.print("Pressure = ");
		Serial.print(bme.readPressure() / 100.0F);
		Serial.println(" hPa");
		
		Serial.print("Approx. Altitude = ");
		Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
		Serial.println(" m");
		
		Serial.print("Humidity = ");
		Serial.print(bme.readHumidity());
		Serial.println(" %");
		
		Serial.println();
	}
	
	delay(50);
}

