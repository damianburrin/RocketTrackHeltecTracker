
#include <axp20x.h>

#include <LoRa.h>

//#include "FS.h"
//#include "SD.h"

#include "SPI.h"

#include "ConfigFile.h"
#include "SDCard.h"
#include "PressureSensor.h"
#include "Neopixels.h"
#include "Leds.h"
#include "Beeper.h"

// CONFIGURATION SECTION.

#define SCK					5		// GPIO5  -- SX1278's SCK
#define MISO				19		// GPIO19 -- SX1278's MISO
#define MOSI				27		// GPIO27 -- SX1278's MOSI

// HARDWARE DEFINITION

#define SDCARD_MISO			19		// orangey red
#define SDCARD_MOSI			27		// purple
#define SDCARD_SCK			5		// green
#define SDCARD_NSS			4		// yellow

void setup()
{
	// Serial port(s)
	Serial.begin(115200);
	
//	while(!Serial);		// wait for a serial connection
	
	Serial.print("\n--------\tRocketTrack Flight Telemetry System\t--------\r\n\n");
	
	// SPI
	SPI.begin(SCK,MISO,MOSI);
	
	// I2C
	Wire.begin(21,22);
	
	// SD card is optional but if present, modes of operation are configured
	// from a file rather than just compiled-in defaults.  It will also use
	// a more elaborate web page too
	
	// disabled until i build some hardware with this wiring
//	if(SetupSDCard())			{	Serial.print("SD Card Setup failed, disabling ...\r\n");			sdcard_enable=false;	}
	
	// mandatory peripherals
	
	if(SetupPMIC())				{	Serial.print("PMIC Setup failed, halting ...\r\n");					while(1);				}

#if 1
	if(SetupWebServer())		{	Serial.print("Web Server Setup failed, disabling ...\r\n");									}
#endif
#if 1
	// disabled while i'm messing around with the web page
	if(SetupLoRa())				{	Serial.print("LoRa Setup failed, halting ...\r\n");					while(1);				}
	if(SetupGPS())				{	Serial.print("GPS Setup failed, halting ...\r\n");					while(1);				}
	if(SetupCrypto())			{	Serial.print("Crypto Setup failed, halting ...\r\n");				while(1);				}
	if(SetupScheduler())		{	Serial.print("Scheduler Setup failed, halting ...\r\n");			while(1);				}

	// optional peripherals
	if(SetupLEDs())				{	Serial.print("LED Setup failed, halting ...\r\n");					while(1);				}
#endif
#if 0
	// optional peripherals
	
	if(SetupBeeper())			{	Serial.print("Beeper Setup failed, disabling ...\r\n");				beeper_enable=false;	}
	if(SetupNeopixels())		{	Serial.print("Neopixels Setup failed, disabling ...\r\n");			neopixels_enable=false;	}
#endif
#if 0
	if(SetupPressureSensor())	{	Serial.print("Pressure Sensor Setup failed, disabling ...\r\n");	psensor_enable=false;	}
#endif
}

int counter=0;

void loop()
{
	PollPMIC();
	PollGPS();
	PollScheduler();
	PollLoRa();
	PollSerial();
	PollLEDs();
}

void PollSerial(void)
{
	static uint8_t cmd[128];
	static uint16_t cmdptr=0;
	char rxbyte;

	while(Serial.available())
	{ 
		rxbyte=Serial.read();
		
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

	if(OK)	{	Serial.println("ok ...");	}
	else	{	Serial.println("?");	}
}

