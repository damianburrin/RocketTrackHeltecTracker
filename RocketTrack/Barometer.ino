
#define DEBUG 2

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

#include "GpsOnePPS.h"

int baro_enable=1;

bool baro_trigger=false;

char baro_type[32]="Generic";

int baro_rate=100;
int baro_period=1000000;	// 10Hz
int last_baro_time=0;

#define BME_ADDRESS	0x76

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

float baro_temp=0.0f;
float baro_pressure=0.0f;
float baro_height=0.0f;
float baro_humidity=0.0f;

float max_baro_height=0.0f;

int SetupBarometer(void)
{
	if(!bme.begin(BME_ADDRESS))
	{
		Serial.println("BME280 barometer not found, disabling");
		baro_enable=0;
		
		return(1);
	}	
	
	if(baro_rate!=0)
		baro_period=1000/baro_rate;
	
	Serial.print("BME280 barometer configured\r\n");

	last_baro_time=millis_1pps();
	
	return(0);
}

void PollBarometer(void)
{
	if(baro_enable)
	{
		if(baro_trigger)
//			||	(millis_1pps()>(last_baro_time+baro_period))	)
		{
#if DEBUG>2
			Serial.println(millis_1pps());
#endif
			
			baro_temp=bme.readTemperature();
			baro_pressure=bme.readPressure()/100.0F;
			baro_height=bme.readAltitude(SEALEVELPRESSURE_HPA);
			baro_humidity=bme.readHumidity();

			if(max_baro_height<baro_height)
				max_baro_height=baro_height;

#if DEBUG>1
			Serial.print("Temperature = ");			Serial.print(baro_temp);		Serial.print(" *C\t");
			Serial.print("Pressure = ");			Serial.print(baro_pressure);	Serial.print(" hPa\t");
			Serial.print("Approx. Altitude = ");	Serial.print(baro_height);		Serial.print(" m\t");
			Serial.print("Humidity = ");			Serial.print(baro_humidity);	Serial.print(" %\t");
			Serial.println();
#endif
		
			if(logging_enable)
			{
			
			}
		
			last_baro_time=millis_1pps();
			
			if(baro_trigger)
				baro_trigger=false;
		}	
	}
}

int BarometerCommandHandler(uint8_t *cmd,uint16_t cmdptr)
{
	// ignore a single key stroke
	if(cmdptr<=2)	return(0);
	
#if (DEBUG>0)
	Serial.println((char *)cmd);
#endif
	
	int retval=1;
	uint8_t cnt;
	
	switch(cmd[1]|0x20)
	{
		case 'a':	Serial.print("Altitude: ");		Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));	Serial.print(" m\r\n");
					break;
					
		case 'p':	Serial.print("Pressure: ");		Serial.print(bme.readPressure()/100.0F);				Serial.print(" hPa\r\n");
					break;

		case 't':	Serial.print("Temperature: ");	Serial.print(bme.readTemperature());					Serial.print(" *C\r\n");
					break;
		
		case 'h':	Serial.print("Humidity = ");	Serial.print(bme.readHumidity());						Serial.print(" %\r\n");
					break;
		
		case 'r':	Serial.print("Altitude: ");		Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));	Serial.print(" m\r\n");
					Serial.print("Pressure: ");		Serial.print(bme.readPressure()/100.0F);				Serial.print(" hPa\r\n");
					Serial.print("Temperature: ");	Serial.print(bme.readTemperature());					Serial.print(" *C\r\n");
					Serial.print("Humidity = ");	Serial.print(bme.readHumidity());						Serial.print(" %\r\n");
					break;
		
		case '?':	Serial.print("Barometer Test Harness\r\n================\r\n\n");
					Serial.print("a\t-\tRead altitude\r\n");
					Serial.print("p\t-\tRead pressure\r\n");
					Serial.print("t\t-\tRead temperature\r\n");
					Serial.print("h\t-\tRead humidity\r\n");
					Serial.print("r\t-\tRead all sensors\r\n");
					Serial.print("?\t-\tShow this menu\r\n");
					break;
		
		default:	retval=0;
					break;
	}
	
	return(retval);
}

