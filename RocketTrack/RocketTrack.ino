
/*

to be done

-	logging format

-	wire up and integrate sd card

-	wire up sensors

-	check low power

-	flight events (based on baro, not gps altitude, using gps location too)

-	turning off the gps after landing detection 
	
-	fsk backup mode if possible









*/ 

#include <axp20x.h>

#include <LoRa.h>

#include <SPI.h>
#include <Wire.h>

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
//#include "ESP32TimerInterrupt.h"

#include "Accelerometer.h"
#include "Barometer.h"
#include "Beeper.h"
#include "ConfigFile.h"
#include "Display.h"
#include "Gyro.h"
#include "Leds.h"
#include "Neopixels.h"
#include "SDCard.h"
#include "SpiffsSupport.h"
#include "Timers.h"
#include "Webserver.h"
#include "WiFiSupport.h"

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
	
	while(!Serial);		// wait for a serial connection, for development purposes only
	
	Serial.print("\n--------\tRocketTrack Flight Telemetry System\t--------\r\n\n");
	
	
#ifdef ARDUINO_TBeam
	SPI.begin(SCK,MISO,MOSI);
	Wire.begin(21,22);
#else
	SPI.begin();
	Wire.begin();
#endif

	SetupDisplay();
	
#if 0
	while(1)
	{
		byte error, address;
		int nDevices;
		
		Serial.println("Scanning...");
		
		nDevices = 0;
		for(address = 1; address < 127; address++ ) 
		{
			// The i2c_scanner uses the return value of
			// the Write.endTransmisstion to see if
			// a device did acknowledge to the address.
			Wire.beginTransmission(address);
			error = Wire.endTransmission();
			
			if (error == 0)
			{
				Serial.print("I2C device found at address 0x");
				if (address<16) 
					Serial.print("0");
				Serial.print(address,HEX);
				Serial.println("  !");

				nDevices++;
			}
			else if (error==4) 
			{
				Serial.print("Unknown error at address 0x");
				if (address<16) 
					Serial.print("0");
				Serial.println(address,HEX);
			}    
		}
		
		if (nDevices == 0)
			Serial.println("No I2C devices found\n");
		else
			Serial.println("done\n");
		
		delay(5000);           // wait 5 seconds for next scan
	}

	//	I2C device found at address 0x34  !		// AXP192 PMIC
	//	I2C device found at address 0x3C  !		// OLED Display
	//	I2C device found at address 0x68  !		// MPU6050 Accelerometer/Magnetometer/Gyro
	//	I2C device found at address 0x76  !		// BME280 pressure sensor  
#endif
  
	// SD card is optional but if present, modes of operation are configured
	// from a file rather than just compiled-in defaults.  It will also use
	// a more elaborate web page too
	
	if(SetupSDCard())			{	Serial.println("SD Card Setup failed, disabling ...\r\n");					sdcard_enable=0;		}
	if(SetupSPIFFS())			{	Serial.println("SPIFFS Setup failed, disabling ...\r\n");					spiffs_enable=0;		}
	
	ReadConfigFile();
	
	// mandatory peripherals
	
	if(SetupPMIC())				{	Serial.println("PMIC Setup failed, halting ...\r\n");						while(1);				}

#ifdef ARDUINO_ARCH_ESP32
	if(SetupWiFi())				{	Serial.println("WiFi connection failed, disabling ...");					wifi_enable=0;			}
	if(SetupWebServer())		{	Serial.println("Web Server Setup failed, disabling ...");					webserver_enable=0;		}
#else
	wifi_enable=0;
	webserver_enable=0;
#endif
#if 1
	if(SetupAccelerometer())	{	Serial.println("Accelerometer setup failed, disabling ...");				acc_enable=0;			}
	if(SetupGyro())				{	Serial.println("Gyro setup failed, disabling ...");							gyro_enable=0;			}
	if(SetupBarometer())		{	Serial.println("Barometer setup failed, disabling ...");					baro_enable=0;			}
#endif
#if 0
	// disabled while i'm messing around with the web page
	if(SetupLoRa())				{	Serial.println("LoRa Setup failed, halting ...\r\n");						while(1);				}
	if(SetupGPS())				{	Serial.println("GPS Setup failed, halting ...\r\n");						while(1);				}
	if(SetupCrypto())			{	Serial.println("Crypto Setup failed, halting ...\r\n");						while(1);				}
	if(SetupScheduler())		{	Serial.println("Scheduler Setup failed, halting ...\r\n");					while(1);				}

	// optional peripherals
	if(SetupLEDs())				{	Serial.println("LED Setup failed, halting ...\r\n");						while(1);				}
#endif
#if 0
	// optional peripherals
	
	if(SetupBeeper())			{	Serial.println("Beeper Setup failed, disabling ...\r\n");					beeper_enable=0;		}
	if(SetupNeopixels())		{	Serial.println("Neopixels Setup failed, disabling ...\r\n");				neopixels_enable=0;		}
#endif
#if 0
	if(SetupTimers())			{	Serial.println("Timer Setup failed, falling back to software timing ...");	timer_enable=0;			}
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

	PollAccelerometer();
	PollGyro();
	PollBarometer();
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

bool IRAM_ATTR TinerHandler0(void *timerNo)
{



	return(true);
}
