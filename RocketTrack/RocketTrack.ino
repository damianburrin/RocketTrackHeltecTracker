
#include <axp20x.h>

/*------------------------------------------------------------------------------------------------------*\
s|                                                                                                        |
| New tracker code that can be simply rebuilt for various hardware designs.  e.g. serial or i2c GPS,     |
| NTX2B or LoRa radio.  Using LoRa, it can configured to use TDMA in which case it will accept uplinked  |
| messages and/or repeat messages from other balloons.                                                   |
|                                                                                                        |
| Configuration is using #defines near the top of the file.  These control which other modules get       |
| used and linked in, and configure those modules (e.g. radio frequency).                                |
|                                                                                                        |
| V0.00   First stab                                                                                     |
|                                                                                                        |
\*------------------------------------------------------------------------------------------------------*/

//------------------------------------------------------------------------------------------------------

// CONFIGURATION SECTION.

// Power settings
#define POWERSAVING	                      // Comment out to disable GPS power saving

// LORA settings
#define LORA_FREQUENCY	434.650
#define LORA_OFFSET		0         // Frequency to add in kHz to make Tx frequency accurate
#define LORA_ID			0
#define LORA_MODE		0


//------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------

// HARDWARE DEFINITION

#define LORA_NSS			18		// Comment out to disable LoRa code
#define LORA_RESET			14		// Comment out if not connected
#define LORA_DIO0			26

#define SCK					5		// GPIO5  -- SX1278's SCK
#define MISO				19		// GPIO19 -- SX1278's MISO
#define MOSI				27		// GPIO27 -- SX1278's MOSI

#define SDCARD_NSS			4		// GPIO4 for select line to SD Card socket

#define USER_BUTTON			38

//------------------------------------------------------------------------------------------------------

AXP20X_Class axp;

//------------------------------------------------------------------------------------------------------
//
//  Globals

// struct TBinaryPacket
// {
// 	uint8_t 	PayloadIDs;
// 	uint16_t	Counter;
// 	uint16_t	BiSeconds;
// 	float		Latitude;
// 	float		Longitude;
// 	int32_t  	Altitude;
// };  //  __attribute__ ((packed));
// 
// struct TGPS
// {
// 	int Hours, Minutes, Seconds;
// 	unsigned long SecondsInDay;					// Time in seconds since midnight
// 	float Longitude, Latitude;
// 	long Altitude, MinimumAltitude, MaximumAltitude, PreviousAltitude;
// 	unsigned int Satellites;
// 	byte FixType;
// 	byte psm_status;
// 	float InternalTemperature;
// 	float BatteryVoltage;
// 	float ExternalTemperature;
// 	float Pressure;
// 	float AscentRate;
// 	unsigned int BoardCurrent;
// 	unsigned int errorstatus;
// 	byte GPSFlightMode;
// 	TFlightMode FlightMode;
// 	byte PowerMode;
// 	int CutdownStatus;
// 	float PredictedLatitude;
// 	float PredictedLongitude;
// 	float CDA;
// 	int UseHostPosition;
// 	int TimeTillLanding;
// 	float PredictedLandingSpeed;
//  } GPS;
// 
int SentenceCounter=0;
#define SEQUENCE_LENGTH 120

//------------------------------------------------------------------------------------------------------

void setup()
{
	// Serial port(s)
	
	Serial.begin(115200);
	Serial.print("\nRocketTrack Flight Telemetry System\n\n");
	
	Wire.begin(21,22);
	
	SetupPMIC();
	SetupPressureSensor();
	SetupCrypto();
	SetupLEDs();
	SetupLoRa();
	SetupGPS();
}

void loop()
{
#if 0
#if 1
	while(Serial.available())	{	uint8_t ch=Serial.read();	Serial1.write(ch);	}
#endif
#if 1
	if(Serial1.available())		{	uint8_t ch=Serial1.read();	Serial.write(ch);	}
#endif
#else
	PollPMIC();
	PollPressureSensor();
	PollGPS();
	PollLoRa();
//	PollLEDs();
	PollSerial();
#endif
}

void PollSerial(void)
{
	static uint8_t cmd[128];
	static uint16_t cmdptr=0;
	char rxbyte;

	while(Serial.available())
	{ 
		rxbyte=Serial.read();
		
//		Serial.write(rxbyte);
		
		cmd[cmdptr++]=rxbyte;
		
		if((rxbyte=='\r')||(rxbyte=='\n'))
		{
			ProcessCommand(cmd,cmdptr);
			cmdptr=0;
		}
		else if(cmdptr>=sizeof(cmd))
		{
			cmdptr--;
		}
	}
}

void ProcessCommand(uint8_t *cmd,uint16_t cmdptr)
{
	int OK=0;
	
//	Serial.println(cmd[0]);
	
	switch(cmd[0]|0x20)
	{
		case 'g':
					OK=GPSCommandHandler(cmd,cmdptr);
					break;
		
		case 'l':
					OK=LORACommandHandler(cmd,cmdptr);
					break;
		
		default:	// do nothing
					break;
	}

	if(OK)	{	Serial.println("*");	}
	else	{	Serial.println("?");	}
}

int ProcessFieldCommand(char *cmd)
{
	int OK=0;

	return OK;
}
