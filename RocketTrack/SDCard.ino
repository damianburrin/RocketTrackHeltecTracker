
#include "SD.h"

#define SDCARD_MISO			19		// oran
#define SDCARD_MOSI			27		// purple
#define SDCARD_SCK			5		// green
#define SDCARD_NSS			4		// yellow

bool sdcard_enable=true;

int SetupSDCard(void)
{
	Serial.print("Initializing SD card...");

	if(!SD.begin(10))
	{
		Serial.println("initialization failed!");
		sdcard_enable=0;
		return(1);
	}
	
	Serial.println("initialization done.");	
	
	return(0);
}

