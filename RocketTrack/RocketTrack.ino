
#include <axp20x.h>

//#include "FS.h"
//#include "SD.h"

#include "SPI.h"

//#include "SDCard.h"

#include "PressureSensor.h"
#include "Neopixels.h"
#include "Leds.h"
#include "Beeper.h"

// CONFIGURATION SECTION.

#define SCK					5		// GPIO5  -- SX1278's SCK
#define MISO				19		// GPIO19 -- SX1278's MISO
#define MOSI				27		// GPIO27 -- SX1278's MOSI

#define USER_BUTTON			38

void setup()
{
	// Serial port(s)
	Serial.begin(115200);
	Serial.print("\nRocketTrack Flight Telemetry System\r\n\n");
	
	// SPI
	SPI.begin(SCK,MISO,MOSI);
	
	// I2C
	Wire.begin(21,22);
	
	// mandatory peripherals
	
	if(SetupPMIC())				{	Serial.print("PMIC Setup failed, halting ...\r\n");					while(1);				}
	if(SetupScheduler())		{	Serial.print("Scheduler Setup failed, halting ...\r\n");			while(1);				}
	if(SetupCrypto())			{	Serial.print("Crypto Setup failed, halting ...\r\n");				while(1);				}
	if(SetupLoRa())				{	Serial.print("LoRa Setup failed, halting ...\r\n");					while(1);				}
	if(SetupGPS())				{	Serial.print("GPS Setup failed, halting ...\r\n");					while(1);				}
	if(SetupLEDs())				{	Serial.print("LED Setup failed, halting ...\r\n");					while(1);				}
	
	// optional peripherals
	
//	if(SetupSDCard())			{	Serial.print("SD Card Setup failed, disabling ...\r\n");			sdcard_enable=false;	}
	if(SetupPressureSensor())	{	Serial.print("Pressure Sensor Setup failed, disabling ...\r\n");	psensor_enable=false;	}
	if(SetupBeeper())			{	Serial.print("Beeper Setup failed, disabling ...\r\n");				beeper_enable=false;	}
	if(SetupNeopixels())		{	Serial.print("Neopixels Setup failed, disabling ...\r\n");			neopixels_enable=false;	}
}

void loop()
{
	PollPMIC();
	PollScheduler();
	PollLoRa();
	PollGPS();
//	PollLEDs();

//	PollSDCard();
//	PollPressureSensor();
//	PollBeeper();
//	PollNeopixels();

	PollSerial();
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
			memset(cmd,0,sizeof(cmd));
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
		case 'g':	OK=GPSCommandHandler(cmd,cmdptr);			break;
		case 'l':	OK=LORACommandHandler(cmd,cmdptr);			break;
		case 'p':	OK=PMICCommandHandler(cmd,cmdptr);			break;
		case 'e':	OK=LEDCommandHandler(cmd,cmdptr);			break;
		case 'o':	OK=LongRangeCommandHandler(cmd,cmdptr);		break;
		case 'h':	OK=HighRateCommandHandler(cmd,cmdptr);		break;
		case 'n':	OK=NeopixelCommandHandler(cmd,cmdptr);		break;
		case 'b':	OK=BeeperCommandHandler(cmd,cmdptr);		break;
		
		case '?':	Serial.print("Hacked Test Harness Menu\r\n=================\r\n\n");
					Serial.print("g\t-\tGPS Commands\r\n");
					Serial.print("l\t-\tLoRa Commands\r\n");
					Serial.print("p\t-\tPMIC Commands\r\n");
					Serial.print("h\t-\tHigh Rate Mode Commands\r\n");
					Serial.print("o\t-\tLong Range Mode Commands\r\n");
					Serial.print("e\t-\tLed Commands\r\n");
					Serial.print("n\t-\tNeopixel Commands\r\n");
					Serial.print("b\t-\tBeeper Commands\r\n");
					Serial.print("t\t-\tTransmitter Mode\r\n");
					Serial.print("r\t-\tReceiver Mode\r\n");
					Serial.print("?\t-\tShow this menu\r\n");
					OK=1;
					break;
					
		default:	// do nothing
					break;
	}

	if(OK)	{	Serial.println("*");	}
	else	{	Serial.println("?");	}
}

